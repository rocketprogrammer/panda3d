/* Python interpreter main program for frozen scripts */

#include "Python.h"
#ifdef _WIN32
#include "malloc.h"
#else
#include <sys/mman.h>
#endif

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#endif

#include <stdio.h>
#include <stdint.h>

#if PY_MAJOR_VERSION >= 3
#include <locale.h>

#if PY_MINOR_VERSION < 5
#define Py_DecodeLocale _Py_char2wchar
#endif
#endif

#ifdef MS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern void PyWinFreeze_ExeInit(void);
extern void PyWinFreeze_ExeTerm(void);

static struct _inittab extensions[] = {
    {0, 0},
};

#if PY_MAJOR_VERSION >= 3
#define WIN_UNICODE
#endif
#endif

#if defined(_WIN32) && PY_VERSION_HEX < 0x03060000
static int supports_code_page(UINT cp) {
  /* Shortcut, because we know that these encodings are bundled by default--
   * see FreezeTool.py and Python's encodings/aliases.py */
  if (cp != 0 && cp != 1252 && cp != 367 && cp != 437 && cp != 850 && cp != 819) {
    const struct _frozen *moddef;
    char codec[100];

    /* Check if the codec was frozen into the program.  We can't check this
     * using _PyCodec_Lookup, since Python hasn't been initialized yet. */
    PyOS_snprintf(codec, sizeof(codec), "encodings.cp%u", (unsigned int)cp);

    moddef = PyImport_FrozenModules;
    while (moddef->name) {
      if (strcmp(moddef->name, codec) == 0) {
        return 1;
      }
      ++moddef;
    }
    return 0;
  }

  return 1;
}
#endif

/* Main program */

#ifdef WIN_UNICODE
int Py_FrozenMain(int argc, wchar_t **argv)
#else
int Py_FrozenMain(int argc, char **argv)
#endif
{
    char *p;
    int n, sts = 1;
    int inspect = 0;
    int unbuffered = 0;

#if PY_MAJOR_VERSION >= 3 && !defined(WIN_UNICODE)
    int i;
    char *oldloc;
    wchar_t **argv_copy = NULL;
    /* We need a second copies, as Python might modify the first one. */
    wchar_t **argv_copy2 = NULL;

    if (argc > 0) {
        argv_copy = (wchar_t **)alloca(sizeof(wchar_t *) * argc);
        argv_copy2 = (wchar_t **)alloca(sizeof(wchar_t *) * argc);
    }
#endif

#if defined(MS_WINDOWS) && PY_VERSION_HEX >= 0x03040000 && PY_VERSION_HEX < 0x03060000
    if (!supports_code_page(GetConsoleOutputCP()) ||
        !supports_code_page(GetConsoleCP())) {
      /* Revert to the active codepage, and tell Python to use the 'mbcs'
       * encoding (which always uses the active codepage).  In 99% of cases,
       * this will be the same thing anyway. */
      UINT acp = GetACP();
      SetConsoleCP(acp);
      SetConsoleOutputCP(acp);
      Py_SetStandardStreamEncoding("mbcs", NULL);
    }
#endif

    Py_FrozenFlag = 1; /* Suppress errors from getpath.c */
    Py_NoSiteFlag = 1;
    Py_NoUserSiteDirectory = 1;

    if ((p = Py_GETENV("PYTHONINSPECT")) && *p != '\0')
        inspect = 1;
    if ((p = Py_GETENV("PYTHONUNBUFFERED")) && *p != '\0')
        unbuffered = 1;

    if (unbuffered) {
        setbuf(stdin, (char *)NULL);
        setbuf(stdout, (char *)NULL);
        setbuf(stderr, (char *)NULL);
    }

#if PY_MAJOR_VERSION >= 3 && !defined(WIN_UNICODE)
    oldloc = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "");
    for (i = 0; i < argc; i++) {
        argv_copy[i] = Py_DecodeLocale(argv[i], NULL);
        argv_copy2[i] = argv_copy[i];
        if (!argv_copy[i]) {
            fprintf(stderr, "Unable to decode the command line argument #%i\n",
                            i + 1);
            argc = i;
            goto error;
        }
    }
    setlocale(LC_ALL, oldloc);
#endif

#ifdef MS_WINDOWS
    PyImport_ExtendInittab(extensions);
#endif /* MS_WINDOWS */

    if (argc >= 1) {
#if PY_MAJOR_VERSION >= 3 && !defined(WIN_UNICODE)
        Py_SetProgramName(argv_copy[0]);
#else
        Py_SetProgramName(argv[0]);
#endif
    }

    Py_Initialize();
#ifdef MS_WINDOWS
    PyWinFreeze_ExeInit();
#endif

#if defined(MS_WINDOWS) && PY_VERSION_HEX < 0x03040000
    if (!supports_code_page(GetConsoleOutputCP()) ||
        !supports_code_page(GetConsoleCP())) {
      /* Same hack as before except for Python 2.7, which doesn't seem to have
       * a way to set the encoding ahead of time, and setting PYTHONIOENCODING
       * doesn't seem to work.  Fortunately, Python 2.7 doesn't usually start
       * causing codec errors until the first print statement. */
      PyObject *sys_stream;
      UINT acp = GetACP();
      SetConsoleCP(acp);
      SetConsoleOutputCP(acp);

      sys_stream = PySys_GetObject("stdin");
      if (sys_stream && PyFile_Check(sys_stream)) {
        PyFile_SetEncodingAndErrors(sys_stream, "mbcs", NULL);
      }
      sys_stream = PySys_GetObject("stdout");
      if (sys_stream && PyFile_Check(sys_stream)) {
        PyFile_SetEncodingAndErrors(sys_stream, "mbcs", NULL);
      }
      sys_stream = PySys_GetObject("stderr");
      if (sys_stream && PyFile_Check(sys_stream)) {
        PyFile_SetEncodingAndErrors(sys_stream, "mbcs", NULL);
      }
    }
#endif

    if (Py_VerboseFlag)
        fprintf(stderr, "Python %s\n%s\n",
            Py_GetVersion(), Py_GetCopyright());

#if PY_MAJOR_VERSION >= 3 && !defined(WIN_UNICODE)
    PySys_SetArgv(argc, argv_copy);
#else
    PySys_SetArgv(argc, argv);
#endif

    n = PyImport_ImportFrozenModule("__main__");
    if (n == 0)
        Py_FatalError("__main__ not frozen");
    if (n < 0) {
        PyErr_Print();
        sts = 1;
    }
    else
        sts = 0;

    if (inspect && isatty((int)fileno(stdin)))
        sts = PyRun_AnyFile(stdin, "<stdin>") != 0;

#ifdef MS_WINDOWS
    PyWinFreeze_ExeTerm();
#endif
    Py_Finalize();

#if PY_MAJOR_VERSION >= 3 && !defined(WIN_UNICODE)
error:
    if (argv_copy2) {
        for (i = 0; i < argc; i++) {
#if PY_MINOR_VERSION >= 4
            PyMem_RawFree(argv_copy2[i]);
#else
            PyMem_Free(argv_copy2[i]);
#endif
        }
    }
#endif
    return sts;
}


#if defined(_WIN32) && PY_MAJOR_VERSION >= 3
int wmain(int argc, wchar_t *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
  struct _frozen *blob, *moddef;
  uint64_t begin, end, size;
  int retval;
  FILE *runtime;

#ifdef _WIN32
  wchar_t buffer[2048];
  GetModuleFileNameW(NULL, buffer, 2048);
  runtime = _wfopen(buffer, L"rb");
#elif defined(__FreeBSD__)
  size_t bufsize = 4096;
  char buffer[4096];
  int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
  mib[3] = getpid();
  if (sysctl(mib, 4, (void *)buffer, &bufsize, NULL, 0) == -1) {
    perror("sysctl");
    return 1;
  }
  runtime = fopen(buffer, "rb");
#else
  // Let's hope that it was invoked with the executable name as first arg.
  runtime = fopen(argv[0], "rb");
#endif

  // Get offsets
  fseek(runtime, -8, SEEK_END);
  end = ftell(runtime);
  fread(&begin, 8, 1, runtime);
  size = end - begin;

  // mmap the section indicated by the offset (or malloc/fread on windows)
#ifdef _WIN32
  blob = (struct _frozen *)malloc(size);
  assert(blob != NULL);
  fseek(runtime, (long)begin, SEEK_SET);
  fread(blob, size, 1, runtime);
#else
  blob = (struct _frozen *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(runtime), begin);
  assert(blob != NULL);
#endif

  fclose(runtime);

  // Offset the pointers in the table using the base mmap address.
  moddef = blob;
  while (moddef->name) {
    moddef->name = (char *)((uintptr_t)moddef->name + (uintptr_t)blob);
    if (moddef->code != 0) {
      moddef->code = (unsigned char *)((uintptr_t)moddef->code + (uintptr_t)blob);
    }
    //printf("MOD: %s %p %d\n", moddef->name, (void*)moddef->code, moddef->size);
    moddef++;
  }

  // Run frozen application
  PyImport_FrozenModules = blob;
  retval = Py_FrozenMain(argc, argv);

  // Free resources
#ifdef _WIN32
  free(blob);
#else
  munmap(blob, size);
#endif
  return retval;
}

#ifdef WIN_UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *lpCmdLine, int nCmdShow) {
  return wmain(__argc, __wargv);
}
#elif defined(_WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nCmdShow) {
  return main(__argc, __argv);
}
#endif
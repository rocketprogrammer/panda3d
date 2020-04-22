#include "DNALoader.h"

#include "DNAVisGroup.h"
#include "DNANode.h"
#include "DNAProp.h"
#include "DNASign.h"
#include "DNASignBaseline.h"
#include "DNASignGraphic.h"
#include "DNAWall.h"
#include "DNAWindows.h"
#include "DNALandmarkBuilding.h"
#include "DNAFlatBuilding.h"
#include "DNAData.h"
#include "DNADoor.h"
#include "DNAFlatDoor.h"
#include "DNAStreet.h"
#include "DNAAnimBuilding.h"
#include "DNAAnimProp.h"
#include "DNAInteractiveProp.h"
#include "DNACornice.h"

#include "dnaLexerDefs.h"
#include "dnaParserDefs.h"

#include "texturePool.h"
#include "fontPool.h"
#include "virtualFileSystem.h"
#include "compress_string.h"

DNALoader::DNALoader(): m_cur_comp(nullptr), m_cur_store(nullptr)
{
}

DNALoader::~DNALoader()
{
}

NodePath DNALoader::load_DNA_file(DNAStorage* store, const Filename& file)
{
    load_DNA_file_base(store, file);
    dna_cat.debug() << "load_DNA_file_base completed" << std::endl;

    if (!m_cur_comp)
        return NodePath(); // Empty NodePath

    NodePath np = NodePath("dna");
    m_cur_comp->traverse(np, m_cur_store);

    m_cur_store = nullptr;
    m_cur_comp = nullptr;

    return np;
}

PT(DNAGroup) DNALoader::load_DNA_file_AI(DNAStorage* store, const Filename& file)
{
    load_DNA_file_base(store, file);
    dna_cat.debug() << "load_DNA_file_base completed" << std::endl;
    m_cur_store = nullptr;
    return m_cur_comp;
}

#define ADDCASE(CODE, CLASS) case CODE:{new_comp=new CLASS("unnamed_comp");dna_cat.debug()<<#CLASS<<std::endl;};break;

void DNALoader::load_DNA_file_base(DNAStorage* store, const Filename& file)
{
    dna_cat.info() << "loading " << file << std::endl;

    static VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();
    Filename found(file);
    vfs->resolve_filename(found, get_model_path());

    if (!vfs->exists(found))
    {
        dna_cat.error() << "unable to find " << file << std::endl;
        return;
    }

    m_cur_comp = nullptr;
    m_cur_store = store;

    std::istream* in = vfs->open_read_file(found, true);
    if (!in)
    {
        dna_cat.error() << "unable to read " << file << std::endl;
        return;
    }

    m_cur_comp = new DNAData("root");
    dna_init_parser(*in, found, this, m_cur_store, m_cur_comp);
    dnayyparse();
    dna_cleanup_parser();
    vfs->close_read_file(in);

    if (dna_error_count() != 0)
        m_cur_comp = nullptr;
}

PT(PandaNode) load_DNA_file(DNAStorage* store, const Filename& file, CoordinateSystem, int)
{
    DNALoader loader;
    PT(DNAGroup) root = loader.load_DNA_file_AI(store, file);
    if (root == nullptr)
        return nullptr;

    nassertr(root->get_type() == DNAData::get_class_type(), nullptr);

    NodePath np("dna");
    root->traverse(np, store);

    PT(PandaNode) result = np.node();

    // Omit "dna" root node, if possible
    if (result->get_num_children() == 1)
        result = result->get_child(0);

    return result;
}

PT(DNAGroup) load_DNA_file_AI(DNAStorage* store, const Filename& file, CoordinateSystem)
{
    DNALoader loader;
    return loader.load_DNA_file_AI(store, file);
}

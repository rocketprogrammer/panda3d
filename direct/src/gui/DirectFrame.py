"""A DirectFrame is a basic DirectGUI component that acts as the base
class for various other components, and can also serve as a basic
container to hold other DirectGUI components.

A DirectFrame can have:

* A background texture (pass in path to image, or Texture Card)
* A midground geometry item (pass in geometry)
* A foreground text Node (pass in text string or OnscreenText)

Each of these has 1 or more states.  The same object can be used for
all states or each state can have a different text/geom/image (for
radio button and check button indicators, for example).

See the :ref:`directframe` page in the programming manual for a more in-depth
explanation and an example of how to use this class.
"""

__all__ = ['DirectFrame']

from panda3d.core import *
from . import DirectGuiGlobals as DGG
from .DirectGuiBase import *
from .OnscreenImage import OnscreenImage
from .OnscreenGeom import OnscreenGeom
from .OnscreenText import OnscreenText


class DirectFrame(DirectGuiWidget):
    DefDynGroups = ('text', 'geom', 'image')
    def __init__(self, parent = None, **kw):
        # Inherits from DirectGuiWidget
        optiondefs = (
            # Define type of DirectGuiWidget
            ('pgFunc',          PGItem,     None),
            ('numStates',       1,          None),
            ('state',           self.inactiveInitState, None),
            # Frame can have:
            # A background texture
            ('image',           None,       self.setImage),
            # A midground geometry item
            ('geom',            None,       self.setGeom),
            # A foreground text node
            ('text',            None,       self.setText),
            # Change default value of text mayChange flag from 0
            # (OnscreenText.py) to 1
            ('textMayChange',  1,          None),
            )
        # Merge keyword options with default options
        self.defineoptions(kw, optiondefs,
                           dynamicGroups = DirectFrame.DefDynGroups)

        # Initialize superclasses
        DirectGuiWidget.__init__(self, parent)

        # Call option initialization functions
        self.initialiseoptions(DirectFrame)

    def __reinitComponent(self, name, component_class, states, **kwargs):
        """Recreates the given component using the given keyword args."""
        assert name in ("geom", "image", "text")

        # constants should be local to or default arguments of constructors
        for c in range(self['numStates']):
            component_name = name + str(c)

            try:
                state = states[c]
            except IndexError:
                state = states[-1]

            if self.hascomponent(component_name):
                if state is None:
                    self.destroycomponent(component_name)
                else:
                    self[component_name + "_" + name] = state
            else:
                if state is None:
                    return

                kwargs[name] = state
                self.createcomponent(
                    component_name,
                    (),
                    name,
                    component_class,
                    (),
                    parent=self.stateNodePath[c],
                    **kwargs
                )

    def clearText(self):
        self['text'] = None
        self.setText()

    def setText(self, text=None):
        if text is not None:
            self["text"] = text

        text = self["text"]
        if text is None or isinstance(text, str):
            text_list = (text,) * self['numStates']
        else:
            # Otherwise, hope that the user has passed in a tuple/list
            textList = self['text']
        # Create/destroy components
        for i in range(self['numStates']):
            component = 'text' + repr(i)
            # If fewer items specified than numStates,
            # just repeat last item
            try:
                text = textList[i]
            except IndexError:
                text = textList[-1]

            if self.hascomponent(component):
                if text == None:
                    # Destroy component
                    self.destroycomponent(component)
                else:
                    self[component + '_text'] = text
            else:
                if text == None:
                    return
                else:
                    from .OnscreenText import OnscreenText
                    self.createcomponent(
                        component, (), 'text',
                        OnscreenText,
                        (), parent = self.stateNodePath[i],
                        text = text, scale = 1, mayChange = self['textMayChange'],
                        sort = DGG.TEXT_SORT_INDEX,
                        )

    def clearGeom(self):
        self['geom'] = None
        self.setGeom()

    def setGeom(self, geom=None):
        if geom is not None:
            self["geom"] = geom

        geom = self["geom"]
        if geom is None or \
           isinstance(geom, NodePath) or \
           isinstance(geom, str):
            geom_list = (geom,) * self['numStates']
        else:
            # Otherwise, hope that the user has passed in a tuple/list
            geomList = geom

        # Create/destroy components
        for i in range(self['numStates']):
            component = 'geom' + repr(i)
            # If fewer items specified than numStates,
            # just repeat last item
            try:
                geom = geomList[i]
            except IndexError:
                geom = geomList[-1]

            if self.hascomponent(component):
                if geom == None:
                    # Destroy component
                    self.destroycomponent(component)
                else:
                    self[component + '_geom'] = geom
            else:
                if geom == None:
                    return
                else:
                    self.createcomponent(
                        component, (), 'geom',
                        OnscreenGeom,
                        (), parent = self.stateNodePath[i],
                        geom = geom, scale = 1,
                        sort = DGG.GEOM_SORT_INDEX)

    def clearImage(self):
        self['image'] = None
        self.setImage()

    def setImage(self, image=None):
        if image is not None:
            self["image"] = image

        image = self["image"]
        if image is None or \
           isinstance(image, NodePath) or \
           isinstance(image, Texture) or \
           isinstance(image, str) or \
           isinstance(image, Filename) or \
           (len(image) == 2 and \
            isinstance(image[0], str) and \
            isinstance(image[1], str)):
            image_list = (image,) * self['numStates']
        else:
            # Otherwise, hope that the user has passed in a tuple/list
            if ((len(arg) == 2) and
                isinstance(arg[0], stringType) and
                isinstance(arg[1], stringType)):
                # Its a model/node pair of strings
                imageList = (arg,) * self['numStates']
            else:
                # Assume its a list of node paths
                imageList = arg
        # Create/destroy components
        for i in range(self['numStates']):
            component = 'image' + repr(i)
            # If fewer items specified than numStates,
            # just repeat last item
            try:
                image = imageList[i]
            except IndexError:
                image = imageList[-1]

            if self.hascomponent(component):
                if image == None:
                    # Destroy component
                    self.destroycomponent(component)
                else:
                    self[component + '_image'] = image
            else:
                if image == None:
                    return
                else:
                    self.createcomponent(
                        component, (), 'image',
                        OnscreenImage,
                        (), parent = self.stateNodePath[i],
                        image = image, scale = 1,
                        sort = DGG.IMAGE_SORT_INDEX)

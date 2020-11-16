#ifndef _H_DNA_WALL_
#define _H_DNA_WALL_

#include "DNANode.h"

class EXPCL_DNA DNAWall : public DNANode
{
    PUBLISHED:
        DNAWall(const std::string& name);
        ~DNAWall();
        
        WRITE_DNA
        {
            DNANode::write_dna(out, false, indent);
            indent += 1;
            
            if (m_code.size())
                INDENTED_OUT << "code [ \"" << m_code << "\" ]" << std::endl;
            
            if (m_color != LVecBase4f(1, 1, 1, 1))
                INDENTED_OUT << "color [ " << m_color.get_x() << " "
                             << m_color.get_y() << " " << m_color.get_z()
                             << " " << m_color.get_w() << " ]" << std::endl;
            
            if (m_height)
                INDENTED_OUT << "height [ " << m_height << " ]" << std::endl;
            
            if (recursive)
                pack_children_dna(out, indent - 1);
        }
        
        COMP_CODE(COMPCODE_WALL);
        COMP_NAME(wall);

    public:
        virtual void make_from_dgi(DatagramIterator& dgi, DNAStorage* store);
        virtual void traverse(NodePath& np, DNAStorage* store);
        
    PROPERTY(LVecBase4f, color);
    PROPERTY(float, height);
    PROPERTY_STRING(code);
        
    TYPE_HANDLE(DNAWall, DNANode);
};

#endif

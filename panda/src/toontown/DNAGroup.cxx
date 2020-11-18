#include "DNAGroup.h"

TypeHandle DNAGroup::_type_handle;

DNAGroup::DNAGroup(const std::string& name): m_name(name), m_parent(nullptr),
                                             m_vis_group(nullptr)
{
}

DNAGroup::~DNAGroup()
{
}

void DNAGroup::add(PT(DNAGroup) group)
{
    m_children.push_back(group);
}

PT(DNAGroup) DNAGroup::at(size_t index)
{
    return m_children.at(index);
}

size_t DNAGroup::get_num_children()
{
    return m_children.size();
}

void DNAGroup::clear_parent()
{
    m_parent = nullptr;
    m_vis_group = nullptr;
}

void DNAGroup::make_from_dgi(DatagramIterator& dgi, DNAStorage* store)
{
    m_name = dgi.get_string();
}

void DNAGroup::traverse(NodePath& np, DNAStorage* store)
{
    NodePath _np = np.attach_new_node(m_name);
    traverse_children(_np, store);
}

void DNAGroup::raise_code_not_found()
{
    raise_code_not_found("<unknown>");
}

void DNAGroup::raise_code_not_found(const char* code)
{
    dna_cat.fatal() << "code not found: " << code << std::endl;
    nassertv(false);
}

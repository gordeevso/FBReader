#include "LibraryTreeNodes.h"

#include "../library/Library.h"
#include "LibraryNodes.h"

const ZLTypeId AuthorTreeNode::TYPE_ID(FBNode::TYPE_ID);

const ZLTypeId &AuthorTreeNode::typeId() const {
	return TYPE_ID;
}

AuthorTreeNode::AuthorTreeNode() {
	const AuthorList &authors = Library::Instance().authors();
	size_t index = 0;
	for (AuthorList::const_iterator it=authors.begin(); it!=authors.end(); ++it) {
		insert(new AuthorNode(*it), index++);
	}
}

std::string AuthorTreeNode::title() const {
	//TODO add resource for this node
	return ZLResource::resource("toolbar")["byAuthor"]["label"].value();
}

std::string AuthorTreeNode::subtitle() const {
	//TODO add another subtitle here
	return ZLResource::resource("toolbar")["byAuthor"]["label"].value();
}
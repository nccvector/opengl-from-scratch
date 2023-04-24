//
// Created by vector on 23/04/24.
//

#include "Node.h"

unsigned int Node::mIdCounter = 0;

Node::Node( const char* name ) : mName { name }, mId{Node::mIdCounter}, mTransform(glm::mat4(1.0f)) {
  // Increase guid counter
  Node::mIdCounter += 1;
}

Node::~Node() {}

glm::mat4& Node::getTransform() {
  return mTransform;
}
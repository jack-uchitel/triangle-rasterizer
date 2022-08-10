

/* Feel free to read from this struct's members. Write to the isometry through
its accessors. Write to the other members only through the accessors here. */
typedef struct nodeNode nodeNode;
struct nodeNode {
    const meshGLMesh *mesh;
    nodeNode *child, *sibling;
    isoIsometry isometry;
    GLuint auxNum, texNum;
    GLdouble *auxiliaries;
    const texTexture **textures;
};

/* Initializes a scene graph node with the given data. Uniforms are assumed to
come in two flavors: uniform textures and 'auxiliary' uniform 4D vectors, which
play whatever roles the user needs. The isometry is initialized to the trivial
isometry. The auxiliaries and textures are allocated but not initialized. The
mesh is set and cannot be changed. If the mesh is NULL, then the node is assumed
to be an isometry branch node with no drawing itself. Returns error code, which
is 0 if successful. Don't forget to free the resources backing the node using
nodeDestroy when you are finished with it. */
int nodeInitialize(
        nodeNode *node, const meshGLMesh *mesh, GLuint auxNum, GLuint texNum,
        const nodeNode *child, const nodeNode *sibling) {
    node->mesh = mesh;
    node->child = (nodeNode *)child;
    node->sibling = (nodeNode *)sibling;
    double rotation[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    double translation[3] = {0.0, 0.0, 0.0};
    isoSetRotation(&(node->isometry), rotation);
    isoSetTranslation(&(node->isometry), translation);
    if (mesh == NULL) {
        node->auxiliaries = NULL;
        node->textures = NULL;
        node->auxNum = 0;
        node->texNum = 0;
        return 0;
    } else {
        node->auxiliaries = (GLdouble *)malloc(auxNum * 4 * sizeof(GLdouble) +
            texNum * sizeof(texTexture *));
        if (node->auxiliaries == NULL)
            return 1;
        node->textures = (const texTexture **)&(node->auxiliaries[auxNum * 4]);
        node->auxNum = auxNum;
        node->texNum = texNum;
        return 0;
    }
}

/* Frees the resources backing the node itself. Also calls nodeDestroy
recursively on the younger siblings and children. Does not free the node's mesh
or textures (because those might still be in use by other nodes). */
void nodeDestroy(nodeNode *node) {
    if (node->auxiliaries != NULL) {
        free(node->auxiliaries);
        node->auxiliaries = NULL;
    }
    node->child = NULL;
    node->sibling = NULL;
}

/* Sets the node's first-child node. Can be NULL. */
void nodeSetChild(nodeNode *node, const nodeNode *child) {
    node->child = (nodeNode *)child;
}

/* Sets the node's next-sibling node. Can be NULL. */
void nodeSetSibling(nodeNode *node, const nodeNode *sibling) {
    node->sibling = (nodeNode *)sibling;
}

/* Sets one of the node's textures. */
void nodeSetTexture(nodeNode *node, GLuint index, const texTexture *tex) {
    if (index < node->texNum)
        node->textures[index] = tex;
}

/* Sets one of the node's auxiliary uniforms. Each auxiliary is a 4D vector. */
void nodeSetAuxiliary(nodeNode *node, GLuint index, const GLdouble value[4]) {
    if (index < node->auxNum)
        vecCopy(4, value, &(node->auxiliaries[index * 4]));
}

/* Given a node, its parent's modeling isometry, the location for the 4x4
modeling isometry, and the correct number of uniform 4D vector locations and
texture locations, renders the node. Also recursively renders its younger
siblings and children. For the root node of the scene graph, pass the identity
as the parent isometry. Per nodeInitialize, the mesh can be NULL, in which case
the node does no rendering itself, but can still affect the isometries of its
descendants. Assumes that no more than 8 textures are being used. */
void nodeRender(
        const nodeNode *node, const GLdouble parent[4][4], GLint modelingLoc,
        GLint auxLocs[], GLint texLocs[]) {
    if (node->texNum > 8) {
        fprintf(stderr, "nodeRender: more than 8 texture units requested.\n");
        return;
    }
    GLenum textureUnits[8] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2,
        GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7};
    /* !!Student code goes here.!! */
   
    //setting isometry
   
    
    GLdouble newParent[4][4];
    GLdouble isometry[4][4];
    isoGetHomogeneous(&(node->isometry), isometry);
    mat444Multiply(parent, isometry, newParent);
    shaSetUniform44(newParent, modelingLoc);
    
    

    //set texture
    //load the auxilaries - load 4 doubles into location. shaSetUniform4
    
    if (node->mesh != NULL){
        for(int i=0; i<node->texNum; i++){
            texRender(node->textures[i], textureUnits[i], i,
                      texLocs[i]);
            //pass the texture, opengl texture unit code, the actual number that corresponds to the code, then the actual location where we want connected.texture unit is not data it is code, it is what operates on the texture and does the calculation. need two texture units when you have 2 textures.
        }
        for(int k=0; k<node->auxNum; k++){
            shaSetUniform4(&(node->auxiliaries[4*k]), auxLocs[k]);
        }
        meshGLRender(node->mesh);
        for(int j=0; j<node->texNum; j++){
            texUnrender(node->textures[j], textureUnits[j]);
            
        }
    }
    
    
    //recursive
    if (node->child != NULL) {
        nodeRender(node->child, newParent, modelingLoc, auxLocs, texLocs);
    }
    if (node->sibling != NULL) {
        nodeRender(node->sibling, parent, modelingLoc, auxLocs, texLocs);
    }
}
    

       
    //keep camera stuff in render?

    
    




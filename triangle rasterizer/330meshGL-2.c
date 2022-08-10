

/* These shortcuts help us dissect buffers sitting in GPU memory. */
#define meshGLDOUBLEOFFSET(bytes) ((GLubyte *)NULL + (bytes * sizeof(GLdouble)))
#define meshGLUINTOFFSET(bytes) ((GLubyte *)NULL + (bytes * sizeof(GLuint)))

/* Feel free to read from this struct's members, but don't write to them except
through the accessor functions. */
typedef struct meshGLMesh meshGLMesh;
struct meshGLMesh {
    GLuint triNum, vertNum, attrDim, vao;
    GLuint vbos[2];
};

/* Initializes the OpenGL mesh from a non-OpenGL base mesh. After this function
completes, the base mesh can be destroyed (because its data have been copied
into GPU memory). When you are done using the OpenGL mesh, don't forget to
deallocate its resources using meshGLDestroy. See also
meshGLFinishInitialization. */

void meshGLInitialize(meshGLMesh *mesh, const meshMesh *base) {
    mesh->triNum = base->triNum;
    mesh->vertNum = base->vertNum;
    mesh->attrDim = base->attrDim;
    /* We need a buffer in GPU memory to store the vertices of our mesh. And we need
    another buffer to store the triangles. These buffers are called vertex buffer
    objects (VBOs). */
    
    /* We also need a vertex array object (VAO), which stores configuration
    information about how the mesh is to be rendered. */
    /* Make the VBOs and fill them with data. GL_STATIC_DRAW tells OpenGL that
    we're going to draw this mesh many times without changing it. Such
    performance hints help OpenGL manage GPU memory efficiently. */
    glGenBuffers(2, mesh->vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertNum * mesh->attrDim * sizeof(GLdouble),
        (GLvoid *)base->vert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->triNum * 3 * sizeof(GLuint),
        (GLvoid *)base->tri, GL_STATIC_DRAW);
    /* Make the VAO. Begin to tell it about the VBOs... */
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[0]);
}

/* Immediately after meshGLInitialize, the user must configure the attributes
using glEnableVertexAttribArray and glVertexAttribPointer. Immediately after
that configuration, the user must call this function to complete the
initialization of the mesh. */
void meshGLFinishInitialization(meshGLMesh *mesh) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbos[1]);
    /* When you're done issuing commands to a VAO, unbind it by binding the
    trivial VAO. (When I first wrote this code, I forgot this step, and it cost
    me several hours of debugging.) */
    glBindVertexArray(0);
   
}

/* Renders the mesh. */
void meshGLRender(const meshGLMesh *mesh) {
   
    /* Draw the scene object using the VBOs and VAO in GPU memory. */
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->triNum * 3, GL_UNSIGNED_INT, meshGLUINTOFFSET(0));
    glBindVertexArray(0);
}

/* Releases the resources backing the mesh. Invoke this function when you are
done using the mesh. */
void meshGLDestroy(meshGLMesh *mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(2, mesh->vbos);
}



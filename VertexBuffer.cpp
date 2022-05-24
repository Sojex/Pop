#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //creates buffer
    GLCall(glGenBuffers(1, &m_RendererID));
    //glbind selects the buffer once bound can control the layout with glVertexAttribPointer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //must define how large it has to be can give data later down the line/ will need dynamic down the line for projhect purposes
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    //glbind selects the buffer once bound can control the layout with glVertexAttribPointer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

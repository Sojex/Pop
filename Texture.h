#pragma once
#include "Renderer.h"
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
						//bits per pixel
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();
				//optional parameter if not set will be zero
	void Bind(unsigned int slot = 0) const;
	void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }


};

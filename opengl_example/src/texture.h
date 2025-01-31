#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "image.h"

CLASS_PTR(Texture)
class Texture {
public:
  // ImagePtr/UPtr이 아닌 Image*를 인자로 쓰는 이유:
  // Texture의 함수가 수행하는 명령에서 딱히 Image의 소유권이 상관x -> 빠른 작업을 위해 Image*
  static TextureUPtr CreateFromImage(const Image* image);
  ~Texture();
  
  const uint32_t Get() const { return m_texture; }
  void Bind() const;
  void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
  void SetWrap(uint32_t sWrap, uint32_t tWrap) const;
    
private:
  Texture() {}
  void CreateTexture();
  void SetTextureFromImage(const Image* image);

  uint32_t m_texture { 0 };
};

#endif // __TEXTURE_H__
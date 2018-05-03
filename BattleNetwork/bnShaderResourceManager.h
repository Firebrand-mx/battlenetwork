#pragma once
#include "bnShaderType.h"
#include "bnMemory.h"
#include "bnLogger.h"

#include <SFML/Graphics.hpp>
using sf::Texture;
using sf::Font;
using std::string;
#include <map>
using std::map;
using std::pair;
#include <vector>
using std::vector;
#include <iostream>
using std::cerr;
using std::endl;

class ShaderResourceManager {
public:
  static ShaderResourceManager& GetInstance();
  void LoadAllShaders (unsigned &status);
  sf::Shader* LoadShaderFromFile(string _path);
  sf::Shader* GetShader(ShaderType _ttype);

private:
  ShaderResourceManager(void);
  ~ShaderResourceManager(void);
  vector<string> paths;
  map<ShaderType, sf::Shader*> shaders;
};
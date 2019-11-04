// shaders.cpp
//

///// Includes /////

#include "monocleclient/shaders.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const QString RGB_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 texcoord;\n"
  "in vec3 position;\n"
  "out vec2 out_texcoord;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0);\n"
  "  out_texcoord = texcoord\n;"
  "}\n";

const QString RGB_PIXEL_SHADER =
  "#version 130\n"
  "in vec2 out_texcoord;\n"
  "uniform sampler2D sampler;\n"
  "out vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  colour = texture(sampler, out_texcoord);\n"
  "}\n";

const QString YUV_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 texcoord;\n"
  "in vec3 position;\n"
  "out vec2 out_texcoord;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0);\n"
  "  out_texcoord = texcoord\n;"
  "}\n";

const QString YUV_PIXEL_SHADER =
  "#version 130\n"
  "in vec2 out_texcoord;\n"
  "uniform sampler2D texY;\n"
  "uniform sampler2D texU;\n"
  "uniform sampler2D texV;\n"
  "out vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  float y = 1.1643 * (texture2D(texY, out_texcoord.st).r - 0.0625);\n"
  "  float u = texture2D(texU, out_texcoord.st).r - 0.5;\n"
  "  float v = texture2D(texV, out_texcoord.st).r - 0.5;\n"
  "  float r = y + 1.5958 * v;\n"
  "  float g = y - 0.39173 * u - 0.81290 * v;\n"
  "  float b = y + 2.017 * u;\n"
  "  colour = vec4(r, g, b, 1.0);\n"
  "}\n";

const QString NV12_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 texcoord;\n"
  "in vec3 position;\n"
  "out vec2 out_texcoord;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0);\n"
  "  out_texcoord = texcoord\n;"
  "}\n";

const QString NV12_PIXEL_SHADER =
  "#version 130\n"
  "in vec2 out_texcoord;\n"
  "uniform sampler2D texY;\n"
  "uniform sampler2D texUV;\n"
  "out vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  float y = texture2D(texY, out_texcoord.st).r;\n"
  "  float u = texture2D(texUV, out_texcoord.st).r - 0.5;\n"
  "  float v = texture2D(texUV, out_texcoord.st).g - 0.5;\n"
  "  float r = y + (1.13983 * v);\n"
  "  float g = y - ((0.39465 * u) + (0.58060 * v));\n"
  "  float b = y + (2.03211 * u);\n"
  "  colour = vec4(r, g, b, 1.0);\n"
  "}\n";

const QString INFO_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 texcoord;\n"
  "in vec3 position;\n"
  "out vec2 out_texcoord;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0);\n"
  "  out_texcoord = texcoord\n;"
  "}\n";

const QString INFO_PIXEL_SHADER =
  "#version 130\n"
  "in vec2 out_texcoord;\n"
  "uniform sampler2D sampler;\n"
  "out vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  colour = texture(sampler, out_texcoord);\n"
  "}\n";

const QString SELECTED_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 position;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0, 1.0);\n"
  "}\n";

const QString SELECTED_PIXEL_SHADER =
  "#version 130\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);\n"
  "}\n";

const QString RECORDINGBLOCKS_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 position;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0, 1.0);\n"
  "}\n";

const QString RECORDINGBLOCKS_PIXEL_SHADER =
  "#version 130\n"
  "uniform vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = colour;\n"
  "}\n";

const QString MARKER_VERTEX_SHADER =
"#version 130\n"
"in vec2 position;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(position, 1.0, 1.0);\n"
"}\n";

const QString MARKER_PIXEL_SHADER =
  "#version 130\n"
  "uniform vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = colour;\n"
  "}\n";

const QString TEXT_VERTEX_SHADER =
  "#version 130\n"
  "in vec2 texcoord;\n"
  "in vec3 position;\n"
  "out vec2 out_texcoord;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(position, 1.0);\n"
  "  out_texcoord = texcoord\n;"
  "}\n";

const QString TEXT_PIXEL_SHADER =
  "#version 130\n"
  "in vec2 out_texcoord;\n"
  "uniform sampler2D sampler;\n"
  "out vec4 colour;\n"
  "void main()\n"
  "{\n"
  "  colour = texture(sampler, out_texcoord);\n"
  "}\n";

}

#ifndef _H_GLRENDEROBJECT
#define _H_GLRENDEROBJECT 

#include <string>
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "GL\glew.h"
#include "GLVertexBuffer.h"
#include "GLRenderNode.h"
#include "GLMaterial.h"
#include <map>

typedef std::map<std::string, std::shared_ptr<GLMaterial>> MaterialMap;

class GLObjLoader
{

public:
        ////////////////////////////////////////////////////////////
        // TODO:  Build a file converter to change ascii obj
        //        files into binary for faster loading.
        ////////////////////////////////////////////////////////////


        static bool         load(const std::string&, std::vector<GLRenderNode*>&);

        // Model loading functions....

        static bool         loadTestObject( std::vector<GLRenderNode*>&);

        static bool         loadTempBuffers( FileManager& stream, std::vector<float>&, std::vector<float>&, std::vector<float>&);

        static void         getMtlName( FileManager&, std::string& );

        static bool         createNodes(FileManager& stream, std::vector<float>& verts, std::vector<float>& tex, std::vector<float>& norms, 
                                        MaterialMap&, std::vector<GLRenderNode*>&);

        static void         splitPolyToTriangles( char * faceIndexString, std::vector<int>& vertIndex,
                                                    std::vector<int>& texIndex, std::vector<int>& normIndex);

        static GLRenderNode* createRenderNode(std::vector<float>&, std::vector<float>&, std::vector<float>&,
                                std::vector<int>&, std::vector<int>&, std::vector<int>&, std::shared_ptr<GLMaterial>&);

        // Material Loading functions....

        static bool         loadMaterials(std::string&, MaterialMap&);

        static void         createDefaultMaterial(MaterialMap& materialMap);

        static std::shared_ptr<GLMaterial> createNewMaterial( FileManager& mtlFileStream );
private:

                            GLObjLoader();

                            ~GLObjLoader();

                            GLObjLoader(const GLObjLoader&);

        GLObjLoader&        operator=(const GLObjLoader&);
};  

#endif
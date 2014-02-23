#include "GLObjLoader.h"
#include "GLRenderOperation.h"
#include <regex>
#include <vector>
#include <memory>
#include "FileManager.h"

bool isNormPresent(char * indexString)
{
    assert(indexString);

    int i = 0;
    int seperatorCount = 0;

    while (indexString[i] != NULL)
    {
        if (indexString[i] == '/')
        {
            seperatorCount++;
        }
        i++;
    }

    // We've encountered 2 instances of '/' meaning a normal is present for this face
    if (seperatorCount == 2)
    {
        return true;
    }

    return false;
}

bool isTexPresent(char * indexString)
{
    assert(indexString);

    int i = 0;

    while ( indexString[i] != NULL )
    {
        // If the string contains a seperator and the following character is a number we know 
        // that there is a tex coord for this face
        if ( indexString[i] == '/' )
        {
            if ( (indexString[i + 1] >= '0' && indexString[i + 1] <= '9') )
            {
                return true;
            }
            else
            {
                return false;
            }

        }
        i++;
    }

    return false;
}

bool GLObjLoader::loadTestObject( std::vector<GLRenderNode*>& renderList)
{
	std::vector<float> vertexVec(9);
	std::vector<int>   indexVec(9);

	vertexVec[0] = -0.8f;
	vertexVec[1] = -0.8f;
	vertexVec[2] = -0.1f;

	vertexVec[3] = 0.0f;
	vertexVec[4] = 0.8f;
	vertexVec[5] = -0.1f;

	vertexVec[6] = 0.8f;
	vertexVec[7] = -0.8f;
	vertexVec[8] = -0.1f;

	indexVec[0] = 0;
	indexVec[1] = 1;
	indexVec[2] = 2;


//    renderList.push_back(createRenderNode(vertexVec, vertexVec, vertexVec, indexVec, indexVec, indexVec));

    return true;
}

bool GLObjLoader::load(const std::string& fileName, std::vector<GLRenderNode*>& sceneNodeList)
{
    // Rewrote this using my C read functions becuase stringstreams in C++ are horrendously slow.

    FileManager objFile(fileName); 
    MaterialMap materialMap;

    std::vector<float> tempVerts;
    std::vector<float> tempTexCoords;
    std::vector<float> tempNormals;


    if ( objFile.getFileStream() == NULL )
    {
        return false;
    }

    std::string mtlFileName;

    GLObjLoader::getMtlName(objFile, mtlFileName);

    std::cout << "MTL filename: "<< mtlFileName << "\n";

    GLObjLoader::createDefaultMaterial( materialMap );

    if ( !GLObjLoader::loadMaterials(mtlFileName, materialMap) )
    {
        std::cout << "Error loading materials in " << __FUNCTION__ << "\n";
        return false;
    }


    // Load all of our vertex data into temp buffers to be used to construct node vertex array from
    // index data
    if( !GLObjLoader::loadTempBuffers(objFile, tempVerts, tempTexCoords, tempNormals) )
    {
        std::cout << "Error loading temp vertex values in " << __FUNCTION__ << "\n";
    }

    if (!GLObjLoader::createNodes(objFile, tempVerts, tempTexCoords, tempNormals, materialMap, sceneNodeList))
    {
        std::cout << "Error creating render nodes \n";
    }

    return true;

}

void GLObjLoader::getMtlName(FileManager& stream, std::string& mtlFileName)
{
    char * pReadLine = NULL;

    // Look for material library filename.  If a v or g is encountered ( model data is found ) or the file reaches the end then it is 
    // assumed there is no material for this object and the material will be set to all white. ( this will be done later).   The stream
    // position will also be reset.

    pReadLine = stream.getNextLine();

    while (pReadLine)
    {
        if (!strncmp(pReadLine, "mtllib", 6))
        {
            mtlFileName.assign(pReadLine + 7, strlen(pReadLine) - 8);
            mtlFileName.insert(0, "resource/");
            return;
        }
        else if (*pReadLine == 'v' || *pReadLine == 'g')
        {
            stream.seekStart();
            return;
        }
        pReadLine = stream.getNextLine();
    }

    stream.seekStart();
}



bool GLObjLoader::loadTempBuffers(FileManager& stream, std::vector<float>& verts, std::vector<float>& tex, std::vector<float>& norms)
{
    char *pReadLine = NULL;

    float temp1, temp2, temp3;

    pReadLine = stream.getNextLine();
        
    while (pReadLine)
    {
        if ( !strncmp(pReadLine, "v ", 2) )
        {
            sscanf_s(pReadLine, "%*s %f %f %f", &temp1, &temp2, &temp3);

            verts.push_back(temp1);
            verts.push_back(temp2);
            verts.push_back(temp3);
        }
        else if ( !strncmp(pReadLine,"vt",2) )
        {
            sscanf_s( pReadLine , "%*s %f %f", &temp1, &temp2 );

            tex.push_back(temp1);
            tex.push_back(temp2);
        }
        else if ( !strncmp(pReadLine, "vn", 2))
        {
            sscanf_s(pReadLine, "%*s %f %f %f", &temp1, &temp2, &temp3);

            norms.push_back(temp1);
            norms.push_back(temp2);
            norms.push_back(temp3);
        }

        pReadLine = stream.getNextLine();
    }

    std::cout << "Loaded temp buffers.  No of vertex " << verts.size() << " No of tex coord " << tex.size() << " No of normals " << norms.size() << "\n";

    stream.seekStart();

    return true;
}

bool GLObjLoader::createNodes(FileManager& stream, std::vector<float>& verts, std::vector<float>& tex, std::vector<float>& norms, 
                                MaterialMap& materialMap, std::vector<GLRenderNode*>& sceneNodeList)
{
    GLRenderNode* pNode = NULL;

    std::vector<int> vertIndex;
    std::vector<int> texIndex;
    std::vector<int> normIndex;

    std::string materialString("default");


    char *pReadLine = stream.getNextLine();

    while (pReadLine)
    {
        // If we find a new group or material then we create a new node.
        if ( !strncmp(pReadLine, "g ", 2) || !strncmp(pReadLine, "usemtl", 6) )
        {
            // First render group we have found continue on....
            if (vertIndex.size() == 0)
            {
                if (!strncmp(pReadLine, "usemtl", 6))
                {
                    std::cout << "found material \n";
                    std::cout << pReadLine;
                    materialString.assign(pReadLine + 7, strlen(pReadLine) - 8);
                }

                pReadLine = stream.getNextLine();

                continue;
            }

            // We've encountered a new group in the object file. We need to clear all of the index lists and then
            // build a new render node and push it onto the scene list.

            std::shared_ptr<GLMaterial> material = materialMap[materialString];

            if ( material == NULL )
            {
                // If we failed to load this material load the default material.
                std::cout << "Could not find material " << materialString << "\n";
                material = materialMap["default"];
            }

            pNode = createRenderNode( verts, tex, norms, vertIndex, texIndex, normIndex, material );
           
            if (pNode)
            {
                sceneNodeList.push_back(pNode);
            }

            // Pushed our node onto list now we reset our current state.

            if (!strncmp(pReadLine, "usemtl", 6))
            {
                std::cout << "found material \n";
                std::cout << pReadLine;
                materialString.assign(pReadLine + 7, strlen(pReadLine) - 8);
            }

            vertIndex.clear();
            texIndex.clear();
            normIndex.clear();
        }
        else if ( !strncmp( pReadLine, "f ", 2 ) )
        {
            splitPolyToTriangles(pReadLine, vertIndex, texIndex, normIndex);
        }

        pReadLine = stream.getNextLine();
    }

    if ( vertIndex.size() > 0  )
    {
        pNode = createRenderNode( verts, tex, norms, vertIndex, texIndex, normIndex, materialMap[materialString] );

        if (pNode)
        {
            sceneNodeList.push_back(pNode);
        }
    }

    return true;
}

void GLObjLoader::splitPolyToTriangles(char * faceIndexString, std::vector<int>& vertIndex, std::vector<int>& texIndex, std::vector<int>& normIndex)
{
    std::vector<int> vertsToSplitToTriangles;
    std::vector<int> texToSplitToTriangles;
    std::vector<int> normsToSplitToTriangles;

    int tempVert, tempTex, tempNorm;

    char * nextToken = NULL;
    char * tokenPtr = strtok_s(faceIndexString, " ", &nextToken);

    while (tokenPtr)
    {

        if (*tokenPtr == 'f')
        {
            tokenPtr = strtok_s(NULL, " ", &nextToken);
            continue;
        }
        else if ( isNormPresent(tokenPtr) && isTexPresent(tokenPtr) )
        {
            sscanf_s(tokenPtr, "%d/%d/%d", &tempVert, &tempTex, &tempNorm);

            texToSplitToTriangles.push_back(tempTex - 1);
            normsToSplitToTriangles.push_back(tempNorm - 1);
            vertsToSplitToTriangles.push_back(tempVert - 1);
        }
        else if ( isTexPresent(tokenPtr) )
        {
            sscanf_s(tokenPtr, "%d/%d", &tempVert, &tempTex);

            texToSplitToTriangles.push_back(tempTex - 1);
            vertsToSplitToTriangles.push_back(tempVert - 1);
        }
        else if ( isNormPresent(tokenPtr) )
        {
            sscanf_s(tokenPtr, "%d//%d", &tempVert, &tempNorm);

            normsToSplitToTriangles.push_back(tempNorm - 1);
            vertsToSplitToTriangles.push_back(tempVert - 1);
        }

        tokenPtr = strtok_s(NULL, " ", &nextToken);
    }

    // Loaded face data into temp vectors.  Time to split them into triangles and put them in the index list.

    if (vertsToSplitToTriangles.size() > 2)
    {
        for (unsigned int i = 2; i < vertsToSplitToTriangles.size(); i++)
        {
            vertIndex.push_back(vertsToSplitToTriangles[0]);
            vertIndex.push_back(vertsToSplitToTriangles[i - 1]);
            vertIndex.push_back(vertsToSplitToTriangles[i]);

            if (vertsToSplitToTriangles.size() == texToSplitToTriangles.size())
            {
                texIndex.push_back(texToSplitToTriangles[0]);
                texIndex.push_back(texToSplitToTriangles[i - 1]);
                texIndex.push_back(texToSplitToTriangles[i]);
            }

            if (vertsToSplitToTriangles.size() == normsToSplitToTriangles.size())
            {
                normIndex.push_back(normsToSplitToTriangles[0]);
                normIndex.push_back(normsToSplitToTriangles[i - 1]);
                normIndex.push_back(normsToSplitToTriangles[i]);
            }
        }
    }
}

GLRenderNode* GLObjLoader::createRenderNode(std::vector<float>& tempVerts, std::vector<float>& tempTex, std::vector<float>& tempNorms,
    std::vector<int>& vertIndex, std::vector<int>& texIndex, std::vector<int>& normIndex, std::shared_ptr<GLMaterial>& material)
{
    std::vector<float> vertex;
    std::vector<float> texcoords;
    std::vector<float> normals;

    vertex.reserve(vertIndex.size() * 3);
    for ( unsigned int i = 0; i < vertIndex.size(); i++ )
    {
        vertex.push_back( tempVerts[ (vertIndex[i] * 3) ] );
        vertex.push_back( tempVerts[ (vertIndex[i] * 3) + 1 ] );
        vertex.push_back( tempVerts[ (vertIndex[i] * 3) + 2 ] );
    }

    texcoords.reserve(texIndex.size()* 2);
    for ( unsigned int i = 0; i < texIndex.size(); i++ )
    {
        texcoords.push_back( tempTex[ (texIndex[i] * 2) ]);
        texcoords.push_back( tempTex[ (texIndex[i] * 2) + 1 ]);
    }

    normals.reserve(normIndex.size() * 3);
    for ( unsigned int i = 0; i < normIndex.size(); i++ )
    {
        normals.push_back( tempNorms[ (normIndex[i] * 3) ]);
        normals.push_back( tempNorms[ (normIndex[i] * 3) + 1 ]);
        normals.push_back( tempNorms[ (normIndex[i] * 3) + 2 ]);
    }

    return new GLRenderNode(vertex, texcoords, normals, material, GLRenderNode::TRIANGLES);
}

void GLObjLoader::createDefaultMaterial(MaterialMap& materialMap)
{
    std::shared_ptr<GLMaterial> defaultMat(new GLMaterial);

    defaultMat->genDefaultMaterial();

    materialMap.insert({ "default", defaultMat });
}

bool GLObjLoader::loadMaterials(std::string& mtlFileName, MaterialMap& materialMap)
{
    FileManager mtlFile(mtlFileName,"r");

    char * pReadLine = NULL;

    std::string materialName;

    pReadLine = mtlFile.getNextLine();

    while (pReadLine)
    {
        if ( !strncmp(pReadLine, "newmtl", 6) )
        {  
            materialName.assign( pReadLine + 7, strlen(pReadLine) - 8 );

            materialMap.insert({ materialName, createNewMaterial(mtlFile) });

            continue;
        }


        pReadLine = mtlFile.getNextLine();
    }
    return true;
}



std::shared_ptr<GLMaterial> GLObjLoader::createNewMaterial(FileManager& mtlFileStream)
{
    std::shared_ptr<GLMaterial> pNewMaterial(new GLMaterial());

    char * pReadLine = mtlFileStream.getNextLine();

    char* nextToken = NULL;

    float readFloats[3];

    // While material pointer is valid and we have not hit a new material;
    while ( pReadLine )
    {
        if (!strncmp(pReadLine, "newmtl", 6))
        {
            pNewMaterial->createShader();
            return pNewMaterial;
        }

        char * pToken = strtok_s(pReadLine, " \t", &nextToken);

        if (!strncmp(pToken, "Ka", 2))
        {
            pToken = strtok_s(NULL, " \t", &nextToken);

            for (int i = 0; i < 3 && pToken; i++)
            {
                sscanf_s(pToken, "%f",&readFloats[i]);

                pToken = strtok_s(NULL, " \t", &nextToken);
            }

            pNewMaterial->setAmbientColor(glm::vec3(readFloats[0], readFloats[1], readFloats[2]));
        }
        else if (!strncmp(pToken, "Kd", 2))
        {
            pToken = strtok_s(NULL, " \t", &nextToken);

            for (int i = 0; i < 3 && pToken; i++)
            {
                sscanf_s(pToken, "%f", &readFloats[i]);

                pToken = strtok_s(NULL, " \t", &nextToken);
            }

            pNewMaterial->setDiffuseColor(glm::vec3(readFloats[0], readFloats[1], readFloats[2]));
        }
        else if (!strncmp(pToken, "Ks", 2))
        {
            pToken = strtok_s(NULL, " \t", &nextToken);

            for (int i = 0; i < 3 && pToken; i++)
            {
                sscanf_s(pToken, "%f", &readFloats[i]);

                pToken = strtok_s(NULL, " \t", &nextToken);
            }

            pNewMaterial->setSpecularColor(glm::vec3(readFloats[0], readFloats[1], readFloats[2]));
        }
        else if (!strncmp(pToken, "Ns", 2))
        {
            pToken = strtok_s(NULL, " \t", &nextToken);

            if (pToken)
            {
                sscanf_s(pToken, "%f", &readFloats[0]);

                pNewMaterial->setSpecularCoef(readFloats[0]);
            }
        }
        else if (!strncmp(pToken, "map_Ka", 6))
        {
            std::string ambientTextureName("resource/");

            pToken = strtok_s(NULL, " \t", &nextToken);

            ambientTextureName.append(pToken);

            std::cout << "Loading ambient map " << ambientTextureName << "\n";

            pNewMaterial->setAmbientMap(ambientTextureName);
        }
        else if (!strncmp(pToken, "map_Kd", 6))
        {
            std::string diffuseTextureName("resource/");

            pToken = strtok_s(NULL, " \t", &nextToken);

            diffuseTextureName.append(pToken);

            std::cout << "Loading diffuse map " << diffuseTextureName << "\n";

            pNewMaterial->setDiffuseMap(diffuseTextureName);
        }
        else if (!strncmp(pToken, "map_Ks", 6))
        {
            std::string specularTextureName("resource/");

            pToken = strtok_s(NULL, " \t", &nextToken);

            specularTextureName.append(pToken);

            std::cout << "Loading specular map " << specularTextureName << "\n";

            pNewMaterial->setSpecularMap(specularTextureName);
        }
        else if (!strncmp(pToken, "map_d", 6))
        {
            std::string alphaTextureName("resource/");

            pToken = strtok_s(NULL, " \t", &nextToken);

            alphaTextureName.append(pToken);

            std::cout << "Loading alpha map " << alphaTextureName << "\n";

            pNewMaterial->setAlphaMap(alphaTextureName);
        }
        else if (!strncmp(pToken, "map_bump", 6))
        {
            std::string alphaTextureName("resource/");

            pToken = strtok_s(NULL, " \t", &nextToken);

            alphaTextureName.append(pToken);

            std::cout << "Loading normal map " << alphaTextureName << "\n";

            pNewMaterial->setNormalMap(alphaTextureName);
        }

        pReadLine = mtlFileStream.getNextLine();
    }

    if (!pNewMaterial->isFinalised())
    {
        pNewMaterial->createShader();
    }

    return pNewMaterial;
}
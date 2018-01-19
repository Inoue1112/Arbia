xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material _01_-_Default {
 0.588235;0.588235;0.588235;1.000000;;
 9.999999;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "yuka_2.png";
 }
}

Frame _ {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Box001 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,-6.000000,0.000000,1.000000;;
  }

  Mesh Box001 {
   12;
   5.000000;-6.000000;0.500000;,
   0.000000;-6.000000;0.500000;,
   0.000000;-6.000000;-0.500000;,
   5.000000;-6.000000;-0.500000;,
   5.000000;0.000000;0.500000;,
   0.000000;0.000000;0.500000;,
   0.000000;-6.000000;0.500000;,
   5.000000;-6.000000;0.500000;,
   0.000000;0.000000;0.500000;,
   0.000000;0.000000;-0.500000;,
   0.000000;-6.000000;-0.500000;,
   0.000000;-6.000000;0.500000;;
   6;
   3;0,1,2;,
   3;2,3,0;,
   3;4,5,6;,
   3;6,7,4;,
   3;8,9,10;,
   3;10,11,8;;

   MeshNormals {
    12;
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;;
    6;
    3;0,1,2;,
    3;2,3,0;,
    3;4,5,6;,
    3;6,7,4;,
    3;8,9,10;,
    3;10,11,8;;
   }

   MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    { _01_-_Default }
   }

   MeshTextureCoords c1 {
    12;
    0.921920;0.114700;,
    0.520182;0.114700;,
    0.520182;0.014410;,
    0.921920;0.014410;,
    0.470995;0.636163;,
    0.917391;0.636163;,
    0.917391;0.128219;,
    0.470995;0.128219;,
    0.160400;0.651402;,
    0.423485;0.651402;,
    0.423485;0.125876;,
    0.160400;0.125876;;
   }
  }
 }
}
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

template AnimTicksPerSecond {
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}

template Animation {
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}

template AnimationSet {
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template FloatKeys {
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}


Material _01_-_Default {
 0.588235;0.588235;0.588235;1.000000;;
 9.999999;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "otosiana.png";
 }
}

Frame _ {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Box002 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh Box002 {
   20;
   5.000000;0.000000;0.500000;,
   0.000000;0.000000;0.500000;,
   0.000000;0.000000;-11.500000;,
   5.000000;0.000000;-11.500000;,
   5.000000;0.250000;0.500000;,
   0.000000;0.250000;0.500000;,
   0.000000;0.000000;0.500000;,
   5.000000;0.000000;0.500000;,
   0.000000;0.250000;0.500000;,
   0.000000;0.250000;-11.500000;,
   0.000000;0.000000;-11.500000;,
   0.000000;0.000000;0.500000;,
   0.000000;0.250000;-11.500000;,
   5.000000;0.250000;-11.500000;,
   5.000000;0.000000;-11.500000;,
   0.000000;0.000000;-11.500000;,
   5.000000;0.250000;-11.500000;,
   5.000000;0.250000;0.500000;,
   5.000000;0.000000;0.500000;,
   5.000000;0.000000;-11.500000;;
   10;
   3;0,1,2;,
   3;2,3,0;,
   3;4,5,6;,
   3;6,7,4;,
   3;8,9,10;,
   3;10,11,8;,
   3;12,13,14;,
   3;14,15,12;,
   3;16,17,18;,
   3;18,19,16;;

   MeshNormals {
    20;
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
    -1.000000;0.000000;0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;;
    10;
    3;0,1,2;,
    3;2,3,0;,
    3;4,5,6;,
    3;6,7,4;,
    3;8,9,10;,
    3;10,11,8;,
    3;12,13,14;,
    3;14,15,12;,
    3;16,17,18;,
    3;18,19,16;;
   }

   MeshMaterialList {
    1;
    10;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { _01_-_Default }
   }

   MeshTextureCoords c1 {
    20;
    0.040585;0.491983;,
    0.040585;0.128027;,
    0.979647;0.128027;,
    0.979647;0.491983;,
    0.984147;0.523643;,
    0.040207;0.523642;,
    0.040207;0.587393;,
    0.984147;0.587393;,
    0.978922;0.636669;,
    0.045432;0.636669;,
    0.045432;0.674264;,
    0.978922;0.674264;,
    0.984147;0.523643;,
    0.040207;0.523642;,
    0.040207;0.587393;,
    0.984147;0.587393;,
    0.978922;0.636669;,
    0.045432;0.636669;,
    0.045432;0.674264;,
    0.978922;0.674264;;
   }
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anims_1 {
 

 Animation {
  
  { Box002 }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;0.000000,1.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Anims_2 {
 

 Animation {
  
  { Box002 }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   11200;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;0.000000,1.000000,0.000000,0.000000;;,
   11200;4;0.000000,1.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   11200;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Anims_3 {
 

 Animation {
  
  { Box002 }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;0.000000,1.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}
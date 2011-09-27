
#pragma once

#include "stdafx.h"
#include <vector>

typedef struct SceneGraphNodeData
{
  D3DXMATRIXA16     transform;
  ID3DXEffect*      pEffect;
  IDirect3DDevice9*  pDevice;
  D3DXVECTOR3       vecEye;
  D3DXMATRIXA16     view;
	D3DXMATRIXA16     projection;
} SceneGraphNodeData;

class SceneGraphNode
{
public:
  SceneGraphNode();
  ~SceneGraphNode();


  void AddChildNode(SceneGraphNode* child){
    m_Children.push_back(child);
  }

  std::vector<SceneGraphNode*> GetChildren()
  {
    return m_Children;
  }

  virtual void Update(float elapsedTime) = 0;
  virtual void Action(SceneGraphNodeData* pData) = 0;
private:
 
  std::vector<SceneGraphNode*> m_Children;
};

class TransformNode : public SceneGraphNode
{
public:
  TransformNode();
  ~TransformNode();
  void SetTransformMatrix(const D3DXMATRIXA16 &transform) {
    m_Transform = transform;
  }

  D3DXMATRIXA16 GetTransformMatrix() const {
    return m_Transform;
  }

  void Update(float elapsedTime);
  void Action(SceneGraphNodeData* pData);
private:
  D3DXMATRIXA16 m_Transform;
};

class RootNode : public SceneGraphNode
{
public:
  RootNode();
  ~RootNode();
  void Update(float elapsedTime);
  void Action(SceneGraphNodeData* pData);
};

class GeometryNode : public SceneGraphNode
{
public:
  GeometryNode();
  ~GeometryNode();
  void SetMesh(ID3DXMesh** const mesh) {
    m_hMesh = mesh;
  }
   ID3DXMesh*  GetMesh() const{
    return *m_hMesh;
  }
  void Update(float elapsedTime);
  void Action(SceneGraphNodeData* pData);
private:
  ID3DXMesh **m_hMesh;
};


class SceneGraph
{
public:
  SceneGraph();
  ~SceneGraph();

  void SetEffect(ID3DXEffect* pEffect) {
    m_pEffect = pEffect;
  }

  void SetDevice(IDirect3DDevice9		*pDevice)
  {
    m_pDevice = pDevice;
  }


	int Render();

  void Update(float elapsedTime);
  RootNode* GetRoot(){
    return &m_Root;
  }
private:
  int RenderTraversal(SceneGraphNode* pNode, D3DXMATRIXA16 *pWorld);
  void UpdateTraversal(SceneGraphNode *pNode, float elapsedTime);
  ID3DXEffect* m_pEffect;
  IDirect3DDevice9			*m_pDevice;
  RootNode m_Root;
  D3DXVECTOR3 m_VecEye;
  D3DXMATRIXA16 m_View;
	D3DXMATRIXA16 m_Projection;
};

#pragma once

#include "stdafx.h"
#include <vector>

class SceneGraphNode
{
public:
  SceneGraphNode();
  ~SceneGraphNode();

  void SetMesh(ID3DXMesh** const mesh) {
    m_hMesh = mesh;
  }
   ID3DXMesh*  GetMesh() const{
    return *m_hMesh;
  }

  void SetParent(SceneGraphNode* parent)
  {
    m_pParent = parent;
  }

  SceneGraphNode* GetParent() const {
    return m_pParent;
  }
  void AddChildNode(SceneGraphNode* child){
    child->SetParent(this);
    m_Children.push_back(child);
  }

  std::vector<SceneGraphNode*> GetChildren()
  {
    return m_Children;
  }

  void SetWorldMatrix(const D3DXMATRIXA16 &world) {
    m_World = world;
  }

  D3DXMATRIXA16 GetWorldMatrix() const {
    return m_World;
  }
  
  void Update();
private:
  D3DXMATRIXA16 m_World;

  ID3DXMesh **m_hMesh;

  SceneGraphNode* m_pParent;
  std::vector<SceneGraphNode*> m_Children;
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

  int Render(SceneGraphNode* pNode);
  void Update();
  SceneGraphNode* GetRoot(){
    return &m_Root;
  }
private:
  ID3DXEffect* m_pEffect;
  IDirect3DDevice9			*m_pDevice;
  SceneGraphNode m_Root;
  D3DXVECTOR3 m_VecEye;
  D3DXMATRIXA16 m_View;
	D3DXMATRIXA16 m_Projection;
};
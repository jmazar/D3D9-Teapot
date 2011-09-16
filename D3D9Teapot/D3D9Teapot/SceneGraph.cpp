#include "SceneGraph.h"

//SceneGraphNode

SceneGraphNode::SceneGraphNode()
  : m_pMesh(NULL)
{
  D3DXMatrixIdentity(&m_World);
  m_pParent = NULL;
}

SceneGraphNode::~SceneGraphNode()
{
}

void SceneGraphNode::Update()
{
}

//SceneGraph

SceneGraph::SceneGraph()
  : m_pEffect(NULL)
{
  m_Root.SetParent(NULL);
  	D3DXVECTOR3 vecEye(0.0f, 0.0f, 5.0f);
	D3DXVECTOR3 vecAt (0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH( &m_View, &vecEye, &vecAt, &vecUp );

  D3DXMatrixPerspectiveFovLH(&m_Projection, (40.0f/180.0f)*D3DX_PI, 1.0f, 0.1f, 25.0f);
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Render(SceneGraphNode *pNode)
{
  std::vector<SceneGraphNode*> children = pNode->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    Render(*iterator);
  }

  D3DXMATRIXA16 world = pNode->GetWorldMatrix();
  SceneGraphNode* parent = pNode->GetParent();
  while(NULL != parent)
  {
    world = world * parent->GetWorldMatrix();

    parent = parent->GetParent();
  }


}

void SceneGraph::Update()
{
}


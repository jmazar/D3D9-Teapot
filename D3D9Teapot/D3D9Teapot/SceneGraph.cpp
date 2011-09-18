#include "SceneGraph.h"

//SceneGraphNode

SceneGraphNode::SceneGraphNode()
  : m_hMesh(NULL)
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
  : m_pEffect(NULL),
  m_pDevice(NULL)
{
  m_Root.SetParent(NULL);
  m_VecEye = D3DXVECTOR3(0.0f, 0.0f, 20.0f);

  D3DXVECTOR3 vecAt (0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f);

  D3DXMatrixPerspectiveFovLH(&m_Projection, (40.0f/180.0f)*D3DX_PI, 1.0f, 0.1f, 25.0f);
  D3DXMatrixLookAtLH( &m_View, &m_VecEye, &vecAt, &vecUp );


}

SceneGraph::~SceneGraph()
{
}

int SceneGraph::Render(SceneGraphNode *pNode)
{

  int result = 0;
  int hr = 0;
  std::vector<SceneGraphNode*> children = pNode->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    Render(*iterator);
  }

  if(NULL !=  pNode->GetMesh())
  {

    D3DXMATRIXA16 worldInverse;
    D3DXMATRIXA16 worldView;
    D3DXMATRIXA16 worldViewProjection;
    D3DXMATRIXA16 world = pNode->GetWorldMatrix();
    SceneGraphNode* parent = pNode->GetParent();
    while(NULL != parent)
    {
      world = world * parent->GetWorldMatrix();

      parent = parent->GetParent();


    }
    //Set the effect Matrices
    if (FAILED( hr = m_pEffect->SetMatrix( "g_mWorld", &world ) ) )
      result = 1;

    D3DXMatrixInverse(&worldInverse, 0, &world);
    if (FAILED( hr = m_pEffect->SetMatrix( "g_mWorldInverse", &worldInverse ) ) )
      result = 1;

    if (FAILED( hr = m_pEffect->SetMatrix( "g_mView", &m_View ) ) ) 
      result = 1;

    worldView = world*m_View;
    if (FAILED( m_pEffect->SetMatrix( "g_mWorldView", &worldView ) ) )
      result = 1;

    worldViewProjection = worldView*m_Projection;
    if (FAILED( m_pEffect->SetMatrix( "g_mWorldViewProjection", &worldViewProjection ) ) )
      result = 1;

    if (FAILED( m_pEffect->SetVector( "g_vEyePos", &(D3DXVECTOR4(m_VecEye, 1))) ) )
      result = 1;
        
 


    UINT passes;
    if (FAILED( hr = m_pEffect->Begin(&passes, 0) ) ) 
      result = 1;
    for (UINT i=0; i < passes; i++)
    {
      if (FAILED( hr = m_pEffect->BeginPass(i) ) ) 
        result = 1;
      if (FAILED( hr = pNode->GetMesh()->DrawSubset(0) ) ) 
        result = 1;
      if (FAILED( hr = m_pEffect->EndPass() ) ) 
        result = 1;
    }
    if (FAILED( hr = m_pEffect->End() ) ) 
      result = 1;



  }


  return result | hr;

}

void SceneGraph::Update()
{
}


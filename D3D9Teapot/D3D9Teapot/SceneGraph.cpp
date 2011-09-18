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

void SceneGraphNode::Update(float elapsedTime)
{
  //elapsedTime *= 1000.0f;
  D3DXMATRIXA16 mRotate;
  D3DXMatrixIdentity( &mRotate );
  D3DXMatrixRotationYawPitchRoll( &mRotate, 0.0f, 0.0f, 0.05f );

  m_World =  m_World * mRotate ;
}

//SceneGraph

SceneGraph::SceneGraph()
  : m_pEffect(NULL),
  m_pDevice(NULL)
{
  m_Root.SetParent(NULL);
  m_VecEye = D3DXVECTOR3(2.0f, 2.0f, 10.0f);

  D3DXVECTOR3 vecAt (0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f);

  D3DXMatrixPerspectiveFovLH(&m_Projection, (40.0f/180.0f)*D3DX_PI, 1.0f, 0.1f, 25.0f);
  D3DXMatrixLookAtLH( &m_View, &m_VecEye, &vecAt, &vecUp );


}

SceneGraph::~SceneGraph()
{
}

int SceneGraph::Render()
{
  int result = 0;
  int hr = 0;
  D3DCOLOR color = D3DRGBA(0.0f, 0.0f, 0.0f, 1.0f);
  // Clear the back buffer
  if (FAILED( hr = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , color, 1.0f, 0) ) )
    result = 1;

  //Load/Set appropriate textures and techniques
  if (FAILED( hr = m_pEffect->SetTechnique( "RenderScene_GeometrynLightsOnly" ) ) ) 
    result = 1;
  D3DXMATRIXA16 identity;
  D3DXMatrixIdentity(&identity);
  if( SUCCEEDED( m_pDevice->BeginScene() ) )
  {

    result = RenderTraversal(&m_Root, &identity);

    if (FAILED( hr = m_pDevice->EndScene() ) ) 
      result = 1;
  }
  else 
    result = 1;

  return result | hr;
}

int SceneGraph::RenderTraversal(SceneGraphNode *pNode, D3DXMATRIXA16 *pWorld)
{

  int result = 0;
  int hr = 0;

  D3DXMATRIXA16 world = *pWorld * pNode->GetWorldMatrix();

  if(NULL !=  pNode->GetMesh())
  {

    D3DXMATRIXA16 worldInverse;
    D3DXMATRIXA16 worldView;
    D3DXMATRIXA16 worldViewProjection;

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

  std::vector<SceneGraphNode*> children = pNode->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    RenderTraversal(*iterator, &world);
  }




  return result | hr;

}

void SceneGraph::Update(float elapsedTime)
{
  UpdateTraversal(&m_Root, elapsedTime);
}
void SceneGraph::UpdateTraversal(SceneGraphNode *pNode, float elapsedTime)
{
  pNode->Update(elapsedTime);
  std::vector<SceneGraphNode*> children = pNode->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    UpdateTraversal(*iterator, elapsedTime);
  }

}


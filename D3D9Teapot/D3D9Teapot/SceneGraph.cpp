#include "SceneGraph.h"



////
// RootNode
////

RootNode::RootNode()
  : SceneGraphNode()
{
}

RootNode::~RootNode()
{
}

void RootNode::Update(float elapsedTime)
{
  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Update(elapsedTime);
  }
}

void RootNode::Action(SceneGraphNodeData* pData)
{
  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Action(pData);
  }
}

////
// TransformNode
////


TransformNode::TransformNode()
  : SceneGraphNode()
{
  D3DXMatrixIdentity(&m_Transform);
}

TransformNode::~TransformNode()
{
}

void TransformNode::Update(float elapsedTime)
{
  
  //Yo, update here dogg.
  float movement = 0.01f;

  D3DXMATRIXA16 mRotate;
  D3DXMatrixIdentity( &mRotate );
  D3DXMatrixRotationYawPitchRoll( &mRotate, movement, movement, movement );

  m_Transform =  m_Transform * mRotate ;

  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Update(elapsedTime);
  }
}

void TransformNode::Action(SceneGraphNodeData* pData)
{
  pData->transform = pData->transform * m_Transform;

  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Action(pData);
  }
}

////
// GeometryNode
////

GeometryNode::GeometryNode()
  : SceneGraphNode(),
  m_hMesh(NULL)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::Update(float elapsedTime)
{
  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Update(elapsedTime);
  }
}

void GeometryNode::Action(SceneGraphNodeData* pData)
{
  //Draw my pretties

  int result = 0;
  int hr = 0;

  D3DXMATRIXA16 worldInverse;
  D3DXMATRIXA16 worldView;
  D3DXMATRIXA16 worldViewProjection;

  //Set the effect Matrices
  if (FAILED( hr = pData->pEffect->SetMatrix( "g_mWorld", &pData->transform ) ) )
    result = 1;

  D3DXMatrixInverse(&worldInverse, 0, &pData->transform);
  if (FAILED( hr = pData->pEffect->SetMatrix( "g_mWorldInverse", &worldInverse ) ) )
    result = 1;

  if (FAILED( hr = pData->pEffect->SetMatrix( "g_mView", &pData->view ) ) ) 
    result = 1;

  worldView = pData->transform * pData->view;
  if (FAILED( pData->pEffect->SetMatrix( "g_mWorldView", &worldView ) ) )
    result = 1;

  worldViewProjection = worldView* pData->projection;
  if (FAILED( pData->pEffect->SetMatrix( "g_mWorldViewProjection", &worldViewProjection ) ) )
    result = 1;

  if (FAILED( pData->pEffect->SetVector( "g_vEyePos", &(D3DXVECTOR4(pData->vecEye, 1))) ) )
    result = 1;

  UINT passes;
  if (FAILED( hr = pData->pEffect->Begin(&passes, 0) ) ) 
    result = 1;
  for (UINT i=0; i < passes; i++)
  {
    if (FAILED( hr = pData->pEffect->BeginPass(i) ) ) 
      result = 1;
    if (FAILED( hr = (*m_hMesh)->DrawSubset(0) ) ) 
      result = 1;
    if (FAILED( hr = pData->pEffect->EndPass() ) ) 
      result = 1;
  }
  if (FAILED( hr = pData->pEffect->End() ) ) 
    result = 1;



  std::vector<SceneGraphNode*> children = this->GetChildren();
  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
    iterator != children.end();
    iterator++)
  {
    (*iterator)->Action(pData);
  }
}














//SceneGraphNode

SceneGraphNode::SceneGraphNode()
{
}

SceneGraphNode::~SceneGraphNode()
{
}


//////////////
//SceneGraph
//////////////
SceneGraph::SceneGraph()
  : m_pEffect(NULL),
  m_pDevice(NULL)
{
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
  SceneGraphNodeData data;
  data.pDevice = m_pDevice;
  data.pEffect = m_pEffect;
  data.projection = m_Projection;
  data.transform = identity;
  data.vecEye = m_VecEye;
  data.view = m_View;

  if( SUCCEEDED( m_pDevice->BeginScene() ) )
  {

    m_Root.Action(&data);

    if (FAILED( hr = m_pDevice->EndScene() ) ) 
      result = 1;
  }
  else 
    result = 1;

  return result | hr;
}

//int SceneGraph::RenderTraversal(SceneGraphNode *pNode, D3DXMATRIXA16 *pWorld)
//{
//
//  int result = 0;
//  int hr = 0;
//
//  D3DXMATRIXA16 world = *pWorld * pNode->GetWorldMatrix();
//
//  if(NULL !=  pNode->GetMesh())
//  {
//
//    D3DXMATRIXA16 worldInverse;
//    D3DXMATRIXA16 worldView;
//    D3DXMATRIXA16 worldViewProjection;
//
//    //Set the effect Matrices
//    if (FAILED( hr = m_pEffect->SetMatrix( "g_mWorld", &world ) ) )
//      result = 1;
//
//    D3DXMatrixInverse(&worldInverse, 0, &world);
//    if (FAILED( hr = m_pEffect->SetMatrix( "g_mWorldInverse", &worldInverse ) ) )
//      result = 1;
//
//    if (FAILED( hr = m_pEffect->SetMatrix( "g_mView", &m_View ) ) ) 
//      result = 1;
//
//    worldView = world*m_View;
//    if (FAILED( m_pEffect->SetMatrix( "g_mWorldView", &worldView ) ) )
//      result = 1;
//
//    worldViewProjection = worldView*m_Projection;
//    if (FAILED( m_pEffect->SetMatrix( "g_mWorldViewProjection", &worldViewProjection ) ) )
//      result = 1;
//
//    if (FAILED( m_pEffect->SetVector( "g_vEyePos", &(D3DXVECTOR4(m_VecEye, 1))) ) )
//      result = 1;
//
//    UINT passes;
//    if (FAILED( hr = m_pEffect->Begin(&passes, 0) ) ) 
//      result = 1;
//    for (UINT i=0; i < passes; i++)
//    {
//      if (FAILED( hr = m_pEffect->BeginPass(i) ) ) 
//        result = 1;
//      if (FAILED( hr = pNode->GetMesh()->DrawSubset(0) ) ) 
//        result = 1;
//      if (FAILED( hr = m_pEffect->EndPass() ) ) 
//        result = 1;
//    }
//    if (FAILED( hr = m_pEffect->End() ) ) 
//      result = 1;
//
//  }
//
//  std::vector<SceneGraphNode*> children = pNode->GetChildren();
//  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
//    iterator != children.end();
//    iterator++)
//  {
//    RenderTraversal(*iterator, &world);
//  }
//
//
//
//
//  return result | hr;
//
//}


void SceneGraph::Update(float elapsedTime)
{
  m_Root.Update(elapsedTime);
}


//void SceneGraph::UpdateTraversal(SceneGraphNode *pNode, float elapsedTime)
//{
//  pNode->Update(elapsedTime);
//  std::vector<SceneGraphNode*> children = pNode->GetChildren();
//  for(std::vector<SceneGraphNode*>::iterator iterator = children.begin();
//    iterator != children.end();
//    iterator++)
//  {
//    UpdateTraversal(*iterator, elapsedTime);
//  }
//
//}


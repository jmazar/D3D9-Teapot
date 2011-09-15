//
//#pragma once
//
//#include "stdafx.h"
//#include <vector>
//
//
//
//class SceneGraphNode
//{
//public:
//	SceneGraphNode();
//	~SceneGraphNode();
//
//	void SetMesh(ID3DXMesh* const mesh) {
//		m_pMesh = mesh;
//	}
//	const ID3DXMesh* const GetMesh() const{
//		return m_pMesh;
//	}
//
//	void AddChildNode(SceneGraphNode* child){
//		m_Children.push_back(child);
//	}
//
//	void SetTranslateMatrix(const D3DXMATRIXA16 &translate) {
//		m_Translate = translate;
//	}
//
//	const D3DXMATRIXA16* GetTranslateMatrix() {
//		return &m_Translate;
//	}
//
//	void SetScaleMatrix(const D3DXMATRIXA16 &scale) {
//		m_Scale = scale;
//	}
//	const D3DXMATRIXA16* GetScaleMatrix() const {
//		return &m_Scale;
//	}
//
//	void SetRotateMatrix(const D3DXMATRIXA16 &rotate) {
//		m_Rotate = rotate;
//	}
//	const D3DXMATRIXA16* GetRotateMatrix() const {
//		return &m_Rotate;
//	}
//	
//	void Update();
//private:
//	D3DXMATRIXA16 m_Translate;
//	D3DXMATRIXA16 m_Scale;
//	D3DXMATRIXA16 m_Rotate; // Euler...change to quaternion
//
//	ID3DXMesh *m_pMesh;
//
//	std::vector<SceneGraphNode*> m_Children;
//};
//
//class SceneGraph
//{
//public:
//	SceneGraph();
//	~SceneGraph();
//
//	void SetEffect(ID3DXEffect* pEffect) {
//		m_pEffect = pEffect;
//	}
//
//	void SetViewMatrix(
//
//	void Render(SceneGraphNode* pNode);
//	void Update();
//private:
//	ID3DXEffect* m_pEffect;
//	SceneGraphNode m_Root;
//	
//	D3DXMATRIXA16 m_View;
//	D3DXMATRIXA16 m_Projection;
//
//	std::vector<D3DXMATRIXA16*> m_MatrixStack;
//	
//	D3DXMATRIXA16 CalculateMatrix();
//};
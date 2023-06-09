// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Code based on https://www.codeproject.com/Articles/9887/CStaticTreeCtrl-A-CStatic-derived-custom-Tree-cont
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once
#ifndef CustomizedTreeNode_Header
#define CustomizedTreeNode_Header

#include "WP_APPDefines.h"

#include "MultiviewPrecompile.h"
#include "ContextMenu.h"
#include "DLG_TreeNodeText.h"
#include "VariableLists/VariableTypeLists.h"
#include "NonGUINode.h"

/// <summary>
/// Edited derivable version of CustomTreeControl's Node class <para />(base code from https://www.codeproject.com/Articles/9887/CStaticTreeCtrl-A-CStatic-derived-custom-Tree-cont)
/// <para />NodeCtrl refers to NodeTree holding this class
/// <para />TreeNode refers to derived class's name (for keeping inherited functionality)
/// </summary>
class CustomizedTreeNode : public NonGUINode
{
//protected:
//	typedef CustomizedTreeNode NodeType;
public:
	/// <summary>
	/// This refers to index inside list of button contexts dictionary unless -1 in which case it uses default menu context list
	/// </summary>
	int NBMenuType;
	/// <summary>
	/// Initializes a new instance of the <see cref="CustomizedTreeNode"/> class.
	/// </summary>
	CustomizedTreeNode()
	{
		DisplayName.clear();
		rNode.SetRectEmpty();

		bUseDefaultTextColor = TRUE;

		bOpen = TRUE;

		ParentIndex = EmptyNode;

		NBMenuType = -1;//Use default node context menu options
		textColor = RGB(240,240,240);//Default to a off-white grey
	}
	/// <summary>
	/// Initializes a new instance of the <see cref="CustomizedTreeNode"/> class.
	/// </summary>
	/// <param name="MenuType">Type of the menu.</param>
	CustomizedTreeNode(int MenuType)
	{
		DisplayName.clear();
		rNode.SetRectEmpty();

		bUseDefaultTextColor = TRUE;

		bOpen = TRUE;

		ParentIndex = EmptyNode;

		NBMenuType = MenuType;//Use non-default node context menu defined inside a (List<CustomOrderedDictionary<string, ButtonContextData>>)? or defined in override function in CustomTreeView
		textColor = RGB(240, 240, 240);
	}

	CustomizedTreeNode(int MenuType, unsigned _int64 parentIndex)
	{
		DisplayName.clear();
		rNode.SetRectEmpty();

		bUseDefaultTextColor = TRUE;

		bOpen = TRUE;

		ParentIndex = parentIndex;

		NBMenuType = MenuType;//Use non-default node context menu defined inside a (List<CustomOrderedDictionary<string, ButtonContextData>>)? or defined in override function in CustomTreeView
		textColor = RGB(240, 240, 240);
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="CustomizedTreeNode"/> class.
	/// </summary>
	virtual ~CustomizedTreeNode()
	{
		DisplayName.clear();
	}

	/// <summary>
	/// The r node
	/// </summary>
	CRect		rNode;

	/// <summary>
	/// The text's color
	/// </summary>
	COLORREF	textColor;
	/// <summary>
	/// The b use default text color
	/// </summary>
	BOOL		bUseDefaultTextColor;

	/// <summary>
	/// The b open
	/// </summary>
	BOOL    bOpen;

	CustomizedTreeNode(std::string newNodeName) : CustomizedTreeNode()
	{
		this->DisplayName = newNodeName;
	}

	/// <summary>
	/// Implements the operator ConvertedType* operator.
	/// </summary>
	/// <returns>The result of the operator.</returns>
	template <typename ConvertedType>
	explicit operator ConvertedType*()
	{
		if (self == nullptr)
		{
			return nullptr;
		}
		else
		{
			return dynamic_cast<ConvertedType*>(self);
		}
	}
};
#endif

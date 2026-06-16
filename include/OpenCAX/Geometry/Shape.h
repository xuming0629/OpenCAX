#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/ShapeTypes.h>

#include <TopoDS_Shape.hxx>

#include <string>

namespace OpenCAX
{

/**
 * @brief 几何拓扑对象基类
 *
 * Shape 是 OpenCAX Geometry 模块中所有几何实体的统一基类，
 * 对 OpenCASCADE 的 TopoDS_Shape 进行封装。
 *
 * 所有具体几何对象均可继承自该类，例如：
 *
 * - Vertex   （顶点）
 * - Edge     （边）
 * - Wire     （线框）
 * - Face     （面）
 * - Shell    （壳）
 * - Solid    （实体）
 * - Compound （组合体）
 *
 * 主要作用：
 * - 统一 OpenCAX 几何对象接口
 * - 屏蔽 OCC 底层细节
 * - 为布尔运算、网格划分、几何分析提供统一入口
 * - 支持后续 Geometry → Mesh → FEM 流程
 *
 * @note
 * Shape 本身并不表示具体几何类型，
 * 实际类型由内部 TopoDS_Shape 决定。
 */
class OpenCAX_API Shape
{
public:

    /**
     * @brief 默认构造函数
     *
     * 创建一个空 Shape。
     *
     * @post
     * isNull() == true
     */
    Shape();

    /**
     * @brief 使用 OCC Shape 构造
     *
     * @param shape OpenCASCADE 拓扑对象
     */
    explicit Shape(const TopoDS_Shape& shape);

    /**
     * @brief 虚析构函数
     *
     * 支持多态继承。
     */
    virtual ~Shape() = default;

    /**
     * @brief 判断对象是否为空
     *
     * 空对象表示未绑定任何 OCC 几何实体。
     *
     * @return
     * true  : 空对象
     * false : 有效对象
     */
    bool isNull() const;

    /**
     * @brief 判断几何对象是否有效
     *
     * 通常会调用 OCC 的 BRepCheck_Analyzer
     * 或其它拓扑检查方法。
     *
     * @return
     * true  : 几何合法
     * false : 几何存在错误
     */
    bool isValid() const;

    /**
     * @brief 获取拓扑类型
     *
     * 自动将 OCC TopAbs_ShapeEnum
     * 转换为 OpenCAX::ShapeType。
     *
     * @return 拓扑类型
     */
    ShapeType type() const;

    /**
     * @brief 获取拓扑类型名称
     *
     * @return 拓扑类型字符串
     */
    std::string typeName() const;

    /**
     * @brief 清空对象
     *
     * 释放内部 OCC Shape 引用。
     *
     * @post
     * isNull() == true
     */
    void clear();

    /**
     * @brief 重新设置内部 OCC Shape
     *
     * @param shape OpenCASCADE 拓扑对象
     */
    void setOcc(const TopoDS_Shape& shape);

    /**
     * @brief 获取 OCC Shape（可修改）
     *
     * 当需要调用 OCC 原生接口时使用。
     *
     * @return TopoDS_Shape 引用
     */
    TopoDS_Shape& occ();

    /**
     * @brief 获取 OCC Shape（只读）
     *
     * 当需要访问 OCC 数据而不修改时使用。
     *
     * @return OCC Shape 常量引用
     */
    const TopoDS_Shape& occ() const;

protected:

    /**
     * @brief OCC 底层拓扑对象
     *
     * OpenCASCADE 所有几何与拓扑实体的统一基类。
     */
    TopoDS_Shape shape_;
};

} // namespace OpenCAX

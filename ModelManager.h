#pragma once

#include <QString>
#include <QFileInfo>
#include <QDebug>

#include <TopoDS_Shape.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>

class ModelManager
{
public:
    ModelManager();
    ~ModelManager() = default;

    // 导入模型，返回是否成功
    bool importModel(const QString& filePath, TopoDS_Shape& outShape);

    // 导出模型，返回是否成功
    bool exportModel(const TopoDS_Shape& shape, const QString& filePath);
};

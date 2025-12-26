#include "ModelManager.h"

ModelManager::ModelManager()
{
    // 可选：初始化一些成员
}

bool ModelManager::importModel(const QString& filePath, TopoDS_Shape& outShape)
{
    QFileInfo info(filePath);
    if (!info.exists()) return false;

    QString suffix = info.suffix().toLower();
    TopoDS_Shape shape;

    if (suffix == "step" || suffix == "stp")
    {
        STEPControl_Reader reader;
        if (reader.ReadFile(filePath.toStdString().c_str()) != IFSelect_RetDone)
            return false;
        reader.TransferRoots();
        shape = reader.OneShape();
    }
    else if (suffix == "iges" || suffix == "igs")
    {
        IGESControl_Reader reader;
        if (reader.ReadFile(filePath.toStdString().c_str()) != IFSelect_RetDone)
            return false;
        reader.TransferRoots();
        shape = reader.OneShape();
    }
    else
    {
        qDebug() << "不支持的文件类型:" << filePath;
        return false;
    }

    if (shape.IsNull()) return false;

    outShape = shape;
    return true;
}

bool ModelManager::exportModel(const TopoDS_Shape& shape,
                               const QString& filePath)
{
    if (shape.IsNull())
    {
        qDebug() << "[ModelManager] 空 Shape，无法导出";
        return false;
    }

    QFileInfo info(filePath);
    QString suffix = info.suffix().toLower();

    try
    {
        if (suffix == "step" || suffix == "stp")
        {
            STEPControl_Writer writer;

            IFSelect_ReturnStatus transferStatus =
                writer.Transfer(shape, STEPControl_AsIs);

            if (transferStatus != IFSelect_RetDone)
            {
                qDebug() << "[STEP] Transfer 失败";
                return false;
            }

            IFSelect_ReturnStatus writeStatus =
                writer.Write(filePath.toStdString().c_str());

            return writeStatus == IFSelect_RetDone;
        }
        else if (suffix == "iges" || suffix == "igs")
        {
            IGESControl_Writer writer;
            writer.AddShape(shape);
            writer.ComputeModel();

            return writer.Write(filePath.toStdString().c_str());
        }
        else
        {
            qDebug() << "[ModelManager] 不支持的导出格式:" << suffix;
            return false;
        }
    }
    catch (Standard_Failure& e)
    {
        qDebug() << "[ModelManager] OCC 异常:" << e.GetMessageString();
        return false;
    }
}


#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <windows.h>
#include <gl/glu.h>                     /**< 包含gl头文件 */
#include "Vector3.h"                 /**< 包含向量类头文件 */

/** 摄像机类 */
class Camera
{
public:
    /** 构造函数和析构函数 */
    Camera();
    ~Camera();

    /** 获得摄像机状态方法 */
    Vector3 getPosition() { return m_Position; }
    Vector3 getView() { return m_View; }
    Vector3 getUpVector() { return m_UpVector; }
    float   getSpeed() { return m_Speed; }

    /** 设置速度 */
    void    setSpeed(float speed)
    {
        m_Speed = speed;
    }

    /** 设置摄像机的位置, 观察点和向上向量 */
    void setCamera(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);

    /** 旋转摄像机方向 */
    void rotateView(float angle, float X, float Y, float Z);

    /** 根据鼠标设置摄像机观察方向 */
    void setViewByMouse();
    void setViewByMouse(int x, int y);
    void setViewByKey(int x, int y);

    /** 左右摄像机移动 */
    void yawCamera(float speed);

    /** 上下摄像机移动 */
    void pitchCamera(float speed);

    /** 前后移动摄像机 */
    void moveCamera(float speed);

    /** 放置摄像机 */
    void setLook();

private:
    /** 摄像机属性 */
    Vector3        m_Position;      /**< 位置 */
    Vector3        m_View;          /**< 朝向 */
    Vector3        m_UpVector;      /**< 向上向量 */
    float          m_Speed = 0.1;         /**< 速度 */
public:
    int middleX = 0; /**< 得到屏幕宽度的一半 */
    int middleY = 0; /**< 得到屏幕高度的一半 */

};

#endif //__CAMERA_H__
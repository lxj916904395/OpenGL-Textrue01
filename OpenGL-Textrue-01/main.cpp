//
//  main.m
//  OpenGL-Textrue-01
//
//  Created by zhongding on 2018/12/13.
//

#include "GLTools.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLShaderManager.h"
#include "GLMatrixStack.h"
#include "GLTriangleBatch.h"
#include "GLGeometryTransform.h"


#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

GLShaderManager shaderManager;

GLFrustum viewFrustum;

GLFrame cameraFrame;
GLFrame worldFrame;

GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;

GLGeometryTransform transformLine;

//纹理对象
GLuint textures;

//金字塔图形批次类
GLBatch pyramidBatch;

void changeSize(int w, int h){
    glViewport(0, 0, w, h);
    
    viewFrustum.SetPerspective(35.0f, float(w)/(float)h, 1, 500);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    transformLine.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void specailKey(int key , int x, int y){
    
    if (key == GLUT_KEY_UP)
        worldFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    
    if (key == GLUT_KEY_DOWN)
        worldFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    
    
    if (key == GLUT_KEY_LEFT)
        worldFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    
    if (key == GLUT_KEY_RIGHT)
        worldFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    
    glutPostRedisplay();
    
}

void MakePyramid(void){
    /*
     构建图形批次类
     参数1：构成图面的类型，点、线或面
     参数2：顶点数
     参数3：是否需要纹理，0：不需要（默认值），1：需要
     */
    pyramidBatch.Begin(GL_TRIANGLES, 18, 1);
    
    //金字塔底部
    //底部的四边形 = 三角形X + 三角形Y
    //三角形X
    
    /*设置法线
     Normal3f：添加一个表面法线（法线坐标 与 Vertex顶点坐标中的Y轴一致）
     表面法线是有方向的向量，代表表面或者顶点面对的方向（相反的方向）。在多数的关照模式下是必须使用。后面的课程会详细来讲法线的应用
     */
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    /**设置纹理坐标
     MultiTexCoord2f(GLuint texture,GLclampf s,GLclamp t);
     参数1：texture，纹理层次，对于使用存储着色器来进行渲染，设置为0
     参数2：(s,t,r,q对应顶点坐标的x,y,z,w)s：对应顶点坐标中的x坐标
     参数3：t:对应顶点坐标中的y
     */
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    //vBlackLeft点
    pyramidBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    //vBlackRight点
    pyramidBatch.Vertex3f(1.0f, -1.0f, -1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    //vFrontRight点
    pyramidBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    
    //三角形B
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
    pyramidBatch.Vertex3f(-1.0f, -1.0f, 1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3f(-1.0f, -1.0f, -1.0f);
    
    pyramidBatch.Normal3f(0.0f, -1.0f, 0.0f);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
    pyramidBatch.Vertex3f(1.0f, -1.0f, 1.0f);
    
    //塔顶
    M3DVector3f vApex = { 0.0f, 1.0f, 0.0f };
    M3DVector3f vFrontLeft = { -1.0f, -1.0f, 1.0f };
    M3DVector3f vFrontRight = { 1.0f, -1.0f, 1.0f };
    M3DVector3f vBackLeft = { -1.0f, -1.0f, -1.0f };
    M3DVector3f vBackRight = { 1.0f, -1.0f, -1.0f };
    M3DVector3f n;
    
    
    // 金字塔前面
    //三角形：（Apex，vFrontLeft，vFrontRight）
    //纹理坐标设置，参考PPT图6-4图
    /** 获取从三点找到一个法线坐标(三点确定一个面)
     void m3dFindNormal(result,point1, point2,point3);
     参数1：结果
     参数2-4：3个顶点数据
     */
    m3dFindNormal(n, vApex, vFrontLeft, vFrontRight);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontLeft);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontRight);
    
    //金字塔左边
    //三角形：（vApex, vBackLeft, vFrontLeft）
    m3dFindNormal(n, vApex, vBackLeft, vFrontLeft);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackLeft);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontLeft);
    
    //金字塔右边
    //三角形：（vApex, vFrontRight, vBackRight）
    m3dFindNormal(n, vApex, vFrontRight, vBackRight);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vFrontRight);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackRight);
    
    //金字塔后边
    //三角形：（vApex, vBackRight, vBackLeft）
    m3dFindNormal(n, vApex, vBackRight, vBackLeft);
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.5f, 1.0f);
    pyramidBatch.Vertex3fv(vApex);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackRight);
    
    pyramidBatch.Normal3fv(n);
    pyramidBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
    pyramidBatch.Vertex3fv(vBackLeft);
    
    
    
    pyramidBatch.End();
    
}


void renderScene(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    modelViewMatrix.PushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures);
    
    M3DMatrix44f mcamera ;
    //获取观察者矩阵
    cameraFrame.GetCameraMatrix(mcamera);
    //观察者矩阵与模型矩阵相乘
    modelViewMatrix.MultMatrix(mcamera);
    
    M3DMatrix44f mject;
    worldFrame.GetMatrix(mject);
    modelViewMatrix.MultMatrix(mject);
    
    GLfloat vlight[] = {1.0f,0.5f,0.2f};
    GLfloat red[] = {1.0f,1.0f,1.f,1.0f};
   
    /**点光源着色器
     参数1：GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF（着色器标签）
     参数2：模型视图矩阵
     参数3：投影矩阵
     参数4：视点坐标系中的光源位置
     参数5：基本漫反射颜色
     参数6：图形颜色（用纹理就不需要设置颜色。设置为0）
     */
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
                                 transformLine.GetModelViewMatrix(),
                                 transformLine.GetProjectionMatrix(),
                                 vlight, red, 0);
    
    pyramidBatch.Draw();
    
    
    modelViewMatrix.PopMatrix();
    
    
    glutSwapBuffers();
}

#pragma mark ***************** 加载纹理
bool loadTagTexture(const char *tagname, GLenum minFilter, GLenum magFilter, GLenum wrapMode){
    
    GLbyte *pBytes;
    int width, height, components;
    GLenum format;
    
    /*
        读取纹理像素数据
     参数1：纹理文件名
     参数2：纹理宽度地址
     参数3：纹理高度地址
     参数4：文件组件地址
     参数5：文件格式地址
     返回值：图像数据指针
     */
    pBytes = gltReadTGABits(tagname, &width, &height, &components, &format);
    
    if (pBytes == NULL){
        printf("拿不到纹理数据");
        return false;
    }
    
    /*纹理缩小时的过滤方式*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    
    /*纹理放大时的过滤方式*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    /*纹理在x轴的环绕方式*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    
    /*纹理在y轴的环绕方式*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    //3、精密包装像素数据
    //参数1：GL_UNPACK_ALIGNMENT,指定OpenGL如何从数据缓存区中解包图像数据
    //参数2:针对GL_UNPACK_ALIGNMENT 设置的值
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    
    //载入纹理
    //参数1：纹理维度
    //参数2：mip贴图层次
    //参数3：纹理单元存储的颜色成分（从读取像素图是获得）
    //参数4：加载纹理宽
    //参数5：加载纹理高
    //参数6：加载纹理的深度
    //参数7：文件格式地址
    //参数8：像素数据的数据类型（GL_UNSIGNED_BYTE，每个颜色分量都是一个8位无符号整数）
    //参数9：指向纹理图像数据的指针
    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, pBytes);
    
    /*释放纹理数据*/
    free(pBytes);
    
    //只有minFilter 等于以下四种模式，才可以生成Mip贴图
    //GL_NEAREST_MIPMAP_NEAREST具有非常好的性能，并且闪烁现象非常弱
    //GL_LINEAR_MIPMAP_NEAREST常常用于对游戏进行加速，它使用了高质量的线性过滤器
    //GL_LINEAR_MIPMAP_LINEAR 和GL_NEAREST_MIPMAP_LINEAR 过滤器在Mip层之间执行了一些额外的插值，以消除他们之间的过滤痕迹。
    //GL_LINEAR_MIPMAP_LINEAR 三线性Mip贴图。纹理过滤的黄金准则，具有最高的精度。
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR ||
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        
        //加载Mip,纹理生成所有的Mip层
        //参数：GL_TEXTURE_1D、GL_TEXTURE_2D、GL_TEXTURE_3D
        glGenerateMipmap(GL_TEXTURE_2D);
    
    return true;
}


void setup(void){
    
    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    shaderManager.InitializeStockShaders();
    

    //初始化纹理对象
    //参数1：纹理个数
    //参数2：纹理对象指针
    glGenTextures(1, &textures);
    
    //绑定纹理状态
    /*
     参数1：纹理类型，GL_TEXTURE_1D，GL_TEXTURE_2D，GL_TEXTURE_3D
     参数2：纹理对象指针
     */
    glBindTexture(GL_TEXTURE_2D, textures);
    
    /*
     加载纹理
     参数1：纹理文件名
     参数2：缩小纹理时的过滤方式
     参数3：放大纹理时的过滤方式
     参数4:纹理坐标的环绕方式
     */
    loadTagTexture("stone.tga",GL_NEAREST,GL_LINEAR,GL_REPEAT);
    
    //构建金字塔批次类
    MakePyramid();
    
    cameraFrame.MoveForward(-10.f);
    
}


int main(int argc,  char * argv[]) {
    
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("纹理");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specailKey);
    
    GLenum err = glewInit();
    if(GLEW_OK != err){
        return 1;
    }
    setup();
    glutMainLoop();
    
    
    //删除纹理
    glDeleteTextures(1, &textures);
    return 0;
}

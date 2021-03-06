#include <jni.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#include <ctime>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include "ModelLoader.h"

#include "main.h"
#include "TextureETC1.h"


extern "C" {
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_load(JNIEnv* env, jobject obj, jobject jAssetManager, jstring jFileName);
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_zoom(JNIEnv* env, jobject obj, jfloat zoom);
    JNIEXPORT void Java_com_android_gles3jni_GLES3JNILib_rotate(JNIEnv* env, jobject obj, jfloat rotateX, jfloat rotateY);
};

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        delete g_renderer;
        g_renderer = NULL;
    }

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

   g_renderer = new Renderer();
    if (!g_renderer->init()) {
        delete g_renderer;
        g_renderer = NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_load(JNIEnv* env, jobject obj, jobject jAssetManager, jstring jFileName) {
    if (g_renderer) {
        AAssetManager* assetManager = AAssetManager_fromJava(env, jAssetManager);

        const char *fileName = env->GetStringUTFChars(jFileName, NULL ) ;
        char modelFile[256] = "Models/", textureFile[256] = "Textures/";

        strcat(modelFile, fileName);

        size_t fileLength = strlen(fileName);
        strncat(textureFile, fileName, fileLength - 4);
        strcat(textureFile, ".pkm");

        clock_t begin = clock();
        Model* model = ModelLoader::Load(assetManager, modelFile);
        clock_t end = clock();
        double elapsedSeconds = double(end - begin) / CLOCKS_PER_SEC;
        ALOGE("LOADINGTIME %1.6f", elapsedSeconds);

        //model->TextureID = TextureETC1::load(assetManager, "Textures/test.pkm");
        model->TextureID = TextureETC1::load(assetManager, textureFile);

        env->ReleaseStringUTFChars(jFileName, fileName);

        g_renderer->setModel(model);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        g_renderer->render();
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_zoom(JNIEnv* env, jobject obj, jfloat zoom){
    if (g_renderer) {
        g_renderer->zoom(zoom);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_rotate(JNIEnv* env, jobject obj, jfloat rotateX, jfloat rotateY){
    if (g_renderer) {
        g_renderer->rotate(rotateX, rotateY);
    }
}

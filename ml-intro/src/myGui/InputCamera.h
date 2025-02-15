#include <windows.h>
#include <dshow.h>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "imgui.h"
#include <comutil.h>
#include <glad/glad.h>

#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "strmiids.lib")


class InputCamera {
public:
    static std::vector<float> CameraSelectorUI();
    static void CloseCamera() { cap.release(); SetDefault(); }
    static void SetDefault() { selectedCamera = 0; previousCamera = -1; }
private:
    static void UpdateTexture(cv::Mat& frame);
    static std::vector<std::string> GetCameraDevices();
    static cv::Mat CaptureFrame();

    static GLuint textureID;
    static cv::VideoCapture cap; 
    static int selectedCamera;
    static int previousCamera;
};
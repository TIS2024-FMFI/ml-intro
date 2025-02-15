#include "InputCamera.h"

GLuint InputCamera::textureID = 0;
cv::VideoCapture InputCamera::cap;
int InputCamera::selectedCamera = 0;
int InputCamera::previousCamera = -1;

std::vector<std::string> InputCamera::GetCameraDevices() {
    std::vector<std::string> deviceNames;
    deviceNames.push_back("No Camera Selected");

    ICreateDevEnum* pDevEnum = nullptr;
    IEnumMoniker* pEnum = nullptr;

    HRESULT hr = CoInitialize(nullptr);
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pDevEnum);
    if (SUCCEEDED(hr)) {
        hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
        if (hr == S_OK) {
            IMoniker* pMoniker = nullptr;
            while (pEnum->Next(1, &pMoniker, nullptr) == S_OK) {
                IPropertyBag* pPropBag;
                hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
                if (SUCCEEDED(hr)) {
                    VARIANT varName;
                    VariantInit(&varName);
                    hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                    if (SUCCEEDED(hr)) {
                        deviceNames.push_back(std::string(_bstr_t(varName.bstrVal)));
                    }
                    VariantClear(&varName);
                    pPropBag->Release();
                }
                pMoniker->Release();
            }
            pEnum->Release();
        }
        pDevEnum->Release();
    }
    CoUninitialize();
    return deviceNames;
}

cv::Mat InputCamera::CaptureFrame() {
    if (!cap.isOpened()) {
        return cv::Mat::zeros(100, 100, CV_8UC3);
    }
    cv::Mat frame;
    cap.read(frame);
    if (frame.empty()) {
        return cv::Mat::zeros(100, 100, CV_8UC3);
    }
    return frame;
}

void InputCamera::UpdateTexture(cv::Mat& frame) {
    if (frame.empty()) return;


    int center_x = frame.cols / 2;
    int center_y = frame.rows / 2;
    int radius = 25;
    cv::Scalar color = cv::Scalar(0, 0, 255);
    cv::line(frame, cv::Point(center_x+radius, center_y), cv::Point(frame.cols, center_y), color, 2);
    cv::line(frame, cv::Point(0, center_y),               cv::Point(center_x-radius, center_y), color, 2);
    cv::line(frame, cv::Point(center_x, center_y+radius), cv::Point(center_x, frame.rows), color, 2);
    cv::line(frame, cv::Point(center_x, 0), cv::Point(center_x, center_y-radius), color, 2);
    cv::circle(frame, cv::Point(center_x, center_y), radius, color, 2);

    if (textureID == 0) {
        glGenTextures(1, &textureID);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ImGui interface
std::vector<float> InputCamera::CameraSelectorUI() {
    static cv::Mat capturedImage;
    static std::vector<std::string> cameras;

    cameras = GetCameraDevices();

    if (ImGui::BeginCombo("Select Camera", cameras[selectedCamera].c_str())) {
        for (size_t i = 0; i < cameras.size(); i++) {
            bool isSelected = (selectedCamera == i);
            if (ImGui::Selectable(cameras[i].c_str(), isSelected)) {
                selectedCamera = static_cast<int>(i);
                if (selectedCamera > 0 && previousCamera != selectedCamera) {
                    cap.open(selectedCamera - 1, cv::CAP_DSHOW);
                }
                else if (selectedCamera == 0 && cap.isOpened()) {
                    cap.release();
                }
                previousCamera = selectedCamera;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (selectedCamera > 0 && cap.isOpened()) {
        capturedImage = CaptureFrame();
        UpdateTexture(capturedImage);
    }

    if (textureID > 0) {
        ImGui::Text("Captured Image:");
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(200, capturedImage.rows * (200.0f / capturedImage.cols)));
        cv::Vec3b pixel = capturedImage.at<cv::Vec3b>(capturedImage.rows / 2, capturedImage.cols / 2);
        return  { pixel[2] / 255.0f, pixel[1] / 255.0f, pixel[0] / 255.0f };
    }
    return {};
}
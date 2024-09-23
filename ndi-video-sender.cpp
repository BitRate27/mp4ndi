#include <iostream>
#include <string>
#include <Processing.NDI.Lib.h>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_mp4_file>" << std::endl;
        return 1;
    }

    // Initialize NDI
    if (!NDIlib_initialize()) {
        std::cerr << "Failed to initialize NDI." << std::endl;
        return 1;
    }

    // Create an NDI source
    NDIlib_send_create_t NDI_send_create_desc;
    NDI_send_create_desc.p_ndi_name = "NDI Video Sender";

    NDIlib_send_instance_t pNDI_send = NDIlib_send_create(&NDI_send_create_desc);
    if (!pNDI_send) {
        std::cerr << "Failed to create NDI sender." << std::endl;
        return 1;
    }

    // Open the video file
    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file" << std::endl;
        return 1;
    }

    // Get video properties
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    // Create NDI video frame
    NDIlib_video_frame_v2_t NDI_video_frame;
    NDI_video_frame.xres = width;
    NDI_video_frame.yres = height;
    NDI_video_frame.FourCC = NDIlib_FourCC_type_BGRA;
    NDI_video_frame.frame_rate_N = static_cast<int>(fps * 100);
    NDI_video_frame.frame_rate_D = 100;

    cv::Mat frame;
    while (cap.read(frame)) {
        cv::Mat frameBGRA;
        cv::cvtColor(frame, frameBGRA, cv::COLOR_BGR2BGRA);

        NDI_video_frame.p_data = frameBGRA.data;
        NDI_video_frame.line_stride_in_bytes = static_cast<int>(frameBGRA.step);

        // Send the frame
        NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);

        // Add a small delay to maintain the original video frame rate
        cv::waitKey(1000 / static_cast<int>(fps));
    }

    // Clean up
    NDIlib_send_destroy(pNDI_send);
    NDIlib_destroy();

    return 0;
}

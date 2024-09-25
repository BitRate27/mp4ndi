#include <csignal>
#include <atomic>
#include <iostream>
#include <string>
#include <Processing.NDI.Lib.h>
#include <opencv2/opencv.hpp>

std::atomic<bool> keep_running(true);

void signal_handler(int signal) {
    if (signal == SIGINT) {
        keep_running = false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <path_to_mp4_file> [ndi_name]" << std::endl;
        return 1;
    }

    const char* ndi_name = (argc == 3) ? argv[2] : "NDI Video Sender";

    // Initialize NDI
    if (!NDIlib_initialize()) {
        std::cerr << "Failed to initialize NDI." << std::endl;
        std::cerr << "You will need to download and install NDI® Tools to use this player." << std::endl;
        std::cerr << "NDI® is a registered trademark of Vizrt NDI AB https://ndi.video/" << std::endl;
        return 1;
    }

    // Create an NDI source
    NDIlib_send_create_t NDI_send_create_desc;
    NDI_send_create_desc.p_ndi_name = ndi_name;

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


    // Register the signal handler
    std::signal(SIGINT, signal_handler);

    std::cerr << "Playing " << argv[1] << " on NDI output: " << ndi_name << std::endl;
    std::cerr << "Press Ctrl-C to stop" << std::endl;
    std::cerr << "NDI® is a registered trademark of Vizrt NDI AB https://ndi.video/" << std::endl;

    // Send the video frames
    cv::Mat frame;
    while (keep_running) {
        cap.set(cv::CAP_PROP_POS_FRAMES, 0);  // Reset to the first frame
        while (cap.read(frame) && keep_running) {
            cv::Mat frameBGRA;
            cv::cvtColor(frame, frameBGRA, cv::COLOR_BGR2BGRA);

            NDI_video_frame.p_data = frameBGRA.data;
            NDI_video_frame.line_stride_in_bytes = static_cast<int>(frameBGRA.step);

            // Send the frame
            NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);

            // Add a small delay to maintain the original video frame rate
            cv::waitKey(1000 / static_cast<int>(fps));
        }
    }

    // Clean up
    NDIlib_send_destroy(pNDI_send);
    NDIlib_destroy();

    return 0;
}

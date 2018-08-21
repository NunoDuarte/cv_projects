    #include <iostream>
    #include <vector>

    #include <opencv2/core.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/videoio.hpp>
    #include <opencv2/opencv.hpp>
    #include <opencv2/opencv_modules.hpp>
    #include <opencv2/cudaobjdetect.hpp>
    #include <opencv2/cudaimgproc.hpp>


    using namespace cv;
    using namespace std;

    #define __DEBPRINT__    (false)
    #define __SHOWFRAMENO__ (false)
    #define __SHOWRECTSIZE__ (false)
    #define STRLEN_FOR_INFORMATION (1024)

    /* Global variables */
    String vehicle_cascade_OpenCV_ORG_OLDHAAR = "/home/nduarte/software/opencv-3.4.1/data/haarcascades_cuda/haarcascade_fullbody.xml";
    //String vehicle_cascade_HAAR = ""/home/nduarte/software/cascade.xml";
    //String vehicle_cascade_LBP = "/home/ubuntu/Pictures/makotoqnb/cascade/LBP/cascade.xml";
    //String vehicle_cascade_name1 = vehicle_cascade_HAAR;

    String window_name = "Capture - vehicle detection";

    void drawText(Mat & frame)
    {
        putText(frame, "/nHello OpenCV/n",
                Point(20, 50),
                FONT_HERSHEY_COMPLEX, 1, // font face and scale
                Scalar(255, 255, 255), // white
                1, LINE_AA); // line thickness and type
    }



    int main(
        int argc,
        char* argv[]
    )
    {
        //------------------------
        // Should we run with GPU mode ?
        // Should we use HAAR/LBP/OLDHAAR trained data ?
        //------------------------
        bool isGPU = false;
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "gpu") == 0)
            {
                 isGPU = true;
            }
            else if (strcmp(argv[i], "cpu") == 0)
            {
                 isGPU = false;
            }
            else if (strcmp(argv[i], "HAAR") == 0)
            {
                //vehicle_cascade_name1 = vehicle_cascade_HAAR;
            }
            else if (strcmp(argv[i], "LBP") == 0)
            {
                //vehicle_cascade_name1 = vehicle_cascade_LBP;
            }
            else if (strcmp(argv[i], "OLDHAAR") == 0)
            {
                //vehicle_cascade_name1 = vehicle_cascade_OpenCV_ORG_OLDHAAR;
            }
        }

        //------------------------
        // Show OpenCV version.
        //------------------------
        cout << "Built with OpenCV " << CV_VERSION << endl;


        //------------------------
        // Create cascade object and load the cascades for vehicle detection.
        //------------------------
        //cout << "Try to create CascadeClassifier instance with file(" << vehicle_cascade_name1 << ")." << endl;
        Ptr<CascadeClassifier> h_vehicle_cascade;
        Ptr<cuda::CascadeClassifier> d_vehicle_cascade;
        if (isGPU)
        {
            d_vehicle_cascade = cv::cuda::CascadeClassifier::create(vehicle_cascade_OpenCV_ORG_OLDHAAR);
            if (NULL == d_vehicle_cascade)
            {
                cout << "Error creating vehicle cascade 1." << endl;
                return -1;
            }
        }
        else
        {
            h_vehicle_cascade = new CascadeClassifier();
            if (NULL == h_vehicle_cascade)
            {
                cout << "Error creating vehicle cascade 2." << endl;
                return -1;
            }

            bool bResult = h_vehicle_cascade->load(vehicle_cascade_OpenCV_ORG_OLDHAAR);
            if (!bResult)
            {
                cout << "Error loading vehicle cascade 3." << endl;
                return -1;
            }
        }


        //------------------------
        // Open our target video.
        //------------------------
        Mat h_colorframe;
         
        String videoFilename = "/home/nduarte/software/opencv-3.4.1/samples/data/vtest.avi";
        cout << "Try to open video file(" << videoFilename << ")." << endl;
        VideoCapture capture;
        capture.open(videoFilename);
        if(capture.isOpened())
        {
            cout << "Capture is opened" << endl;
             
            int frameNum = capture.get(CAP_PROP_FRAME_COUNT);
            int currentFrame;
            //String dispCurrentFrame = new String(STRLEN_FOR_INFORMATION, ); 
            char* dispDetectedRect = (char*)malloc(STRLEN_FOR_INFORMATION * sizeof(char));
            char* dispCurrentFrame = (char*)malloc(STRLEN_FOR_INFORMATION * sizeof(char));
            if (NULL == dispCurrentFrame)
            {
                cout << "Out of heap!!" << endl;
            }


            //------------------------
            // Setup window
            //------------------------
            namedWindow(window_name, WINDOW_AUTOSIZE);
            setWindowProperty(window_name, WINDOW_GUI_EXPANDED, WINDOW_AUTOSIZE);
            int initialPos = 0;


            //------------------------
            // data for CPU
            //------------------------
            Mat h_colorframe;
            Mat h_grayframe;
            std::vector<Rect> h_vehicles;



            //------------------------
            // data for GPU
            //------------------------
            cuda::GpuMat d_colorframe;
            cuda::GpuMat d_grayframe;
            cuda::GpuMat d_vehicles;


            //------------------------
            // Continue detecting while frame existing.
            //------------------------
            for(;;)
            {
                capture >> h_colorframe;

                if(h_colorframe.empty())
                {
                    break;
                }

                
                //------------------------
                // Convert frame to grayscale and equalizing by Histogram.
                //------------------------
                if (isGPU)
                {
                    d_colorframe.upload(h_colorframe); 
                    if (false)
                    {
                        cuda::cvtColor(d_colorframe, d_grayframe, CV_RGB2GRAY);
                    }
                    else
                    {
                        cuda::cvtColor(d_colorframe, d_grayframe, CV_BGR2GRAY);
                    }
                    cuda::equalizeHist(d_grayframe, d_grayframe);

                }
                else
                {
                    if (false)
                    {
                        cvtColor(h_colorframe, h_grayframe, CV_RGB2GRAY);
                    }
                    else
                    {
                        cvtColor(h_colorframe, h_grayframe, CV_BGR2GRAY);
                    }
                    equalizeHist(h_grayframe, h_grayframe);
                }
    #if __DEBPRINT__
                cout << "Finished converting frame to grayscale and equalizing." << endl;
    #endif

                //------------------------
                // Detect vehicles.
                //------------------------
                try
                {
                    if (isGPU)
                    {
                        d_vehicle_cascade->detectMultiScale(d_grayframe, d_vehicles);
                        d_vehicle_cascade->convert(d_vehicles, h_vehicles);
                    }
                    else
                    {

                        h_vehicle_cascade->detectMultiScale(h_grayframe, h_vehicles);
                        h_vehicle_cascade->detectMultiScale(h_grayframe, h_vehicles, 1.1, 3, 0, Size(30, 30), Size(500, 500));
                    }

    #if __DEBPRINT__
                    cout << "Finished detectMultiScale" << endl;
    #endif

                    // Draw detected rectangle.             
                    for (size_t i = 0; i < h_vehicles.size(); i++)
                    {
    #if __SHOWRECTSIZE__
                        sprintf(dispDetectedRect, "%4d, %4d", h_vehicles[i].width, h_vehicles[i].height);
                        putText(h_colorframe, dispDetectedRect,
                                Point(h_vehicles[i].x, h_vehicles[i].y - 10),
                                FONT_ITALIC , 0.5, // font face and scale
                                Scalar(255, 0, 255), // white
                                1, LINE_AA); // line thickness and type
    #endif
                        rectangle(h_colorframe, h_vehicles[i], Scalar( 255, 0, 255 ), 4, 8, 0 );
                    }




                    //------------------------
                    // Show results.
                    //------------------------
                    Mat result = h_colorframe;
    #if __SHOWFRAMENO__
                    currentFrame = capture.get(CV_CAP_PROP_POS_FRAMES);
                    sprintf(dispCurrentFrame, "Frame No. : %08d", currentFrame);
                    if (isGPU)
                    {
                        strcat(dispCurrentFrame, "  Mode : GPU");
                    }
                    else
                    {
                        strcat(dispCurrentFrame, "  Mode : CPU");
                    }
                    putText(result, dispCurrentFrame,
                            Point(20, 460),
                            FONT_ITALIC , 1, // font face and scale
                            Scalar(255, 255, 255), // white
                            1, LINE_AA); // line thickness and type

    #endif
                    imshow(window_name, result);
                    int keyCode = waitKey(1);
                    if (keyCode != -1)
                    {
                        if ((char)keyCode == 'p')
                        {
                            putText(result, "PAUSED",
                                    Point(200, 300),
                                    FONT_ITALIC , 2, // font face and scale
                                    Scalar(0, 0, 255), // white
                                    3, LINE_AA); // line thickness and type
                            imshow(window_name, result);
                            waitKey(0);
                        }
                        else if ((char)keyCode == 'q') 
                        {
                            break;
                        }
                        else
                        {
                            char dispKeyCode[64];
                            sprintf(dispKeyCode, "Key Code : %04d", keyCode);
                            putText(result, dispKeyCode,
                                    Point(50, 50),
                                    FONT_ITALIC , 1, // font face and scale
                                    Scalar(0, 0, 255), // white
                                    3, LINE_AA); // line thickness and type
                            imshow(window_name, result);
                        }
                    }

                }
                catch (Exception& e)
                {
                    cout << e.msg << endl;
                }


    #if __DEBPRINT__
                cout << "stage 9\n";
    #endif
            }
        }
        else
        {
            cout << "No capture" << endl;
            h_colorframe = Mat::zeros(480, 640, CV_8UC1);
            drawText(h_colorframe);
            imshow("Sample", h_colorframe);
            waitKey(0);
        }

        return 0;
    }

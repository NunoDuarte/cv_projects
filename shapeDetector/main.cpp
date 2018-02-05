//#include "opencv2/opencv.hpp"
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sstream>
#include <lsl_cpp.h>
#include <iostream>
#include <fstream>
#include "json.hpp"

#include "yarp/os/all.h"

using namespace std;
using namespace yarp::os;
//using namespace cv;

//#define LSL_OT
#define LSL_PL1
//#define LSL_PL2


#include <iostream>
#include <fstream>
#include <thread>         // std::thread
#include <vector>

using namespace std;
// for convenience
using json = nlohmann::json;
time_t rawtime;

double sample_ot[15];
string sample_pl1;
string sample_pl2;
std::string strtse01 = "-1";
std::string strtse11 = "-1";
std::string strtse02 = "-1";
std::string strtse12 = "-1";

char ot_new_data_ready=0;
char pl1_new_data_ready=0;
char pl2_new_data_ready=0;




void optitrack_capture()
{
    #ifdef LSL_OT
        double ts_ot,old_timestamp;
        string file_name_ot;
        ofstream sample_ot_file;

        sample_ot_file.precision(17);

        vector<lsl::stream_info> results = lsl::resolve_stream("name","MyOptitrack");
        lsl::stream_inlet inlet_ot(results[0]);

        file_name_ot = "samples_ot";
        file_name_ot.append(ctime (&rawtime));
        file_name_ot.append(".csv");
        sample_ot_file.open (file_name_ot.c_str());

        while(1)
        {
            //sample_ot_file.open ("test_ot2pl.csv");
            ts_ot = inlet_ot.pull_sample(sample_ot,15,0.009);
            if(sample_ot[0]!=old_timestamp)
            {
                old_timestamp = sample_ot[0];
                for (unsigned c=0;c<15;c++)
                {
                    //cout << "\t" << sample_ot[c];

                    sample_ot_file << sample_ot[c];
                    sample_ot_file <<",";
                }

                sample_ot_file << endl;
                //cout << endl;
                ot_new_data_ready = 1;
            }
        }
        sample_ot_file.close();

    #else
        while(1)
            ot_new_data_ready = 1;
    #endif

}

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void pl1_capture()
{
  #ifdef LSL_PL1
      double ts_pl1;
      ofstream sample_pl1_file;
      string file_name_pl1;
      double json_ts;
      string json_np;
      char input_str[10000];
      double ts_e0,ts_e1;
      float sample[3];
      const char *channels[] = {"TS","Nx","Ny"};

      std::size_t found, foundtse0, foundtse1, foundtse2, foundcfe0, foundcfe1, foundcfe2;


      string name = "NormPose2IP";
      string type = "Gaze Norm pose";


      lsl::stream_info info(name,type,3,100,lsl::cf_float32,string(name)+=type);
      info.desc().append_child_value("manufacturer", "Vislab");
      lsl::xml_element chns = info.desc().append_child("channels");

   	  chns.append_child("channel").append_child_value("label",channels[0]).append_child_value("unit","sec").append_child_value("type","timestamp");
      chns.append_child("channel").append_child_value("label",channels[1]).append_child_value("unit","NA").append_child_value("type","normpose_x");
      chns.append_child("channel").append_child_value("label",channels[2]).append_child_value("unit","NA").append_child_value("type","normpose_y");

      lsl::stream_outlet outlet(info);


      Network yarp;
      BufferedPort<Bottle> port;
      Bottle& output = port.prepare();
      port.open("/pupil_gaze_tracker");

      json j_obj;

      sample_pl1_file.precision(17);

      vector<lsl::stream_info> results = lsl::resolve_stream("name","Gaze Python Representation");
      lsl::stream_inlet inlet_pl1(results[0]);

      file_name_pl1 = "samples_pl1";
      file_name_pl1.append(ctime (&rawtime));
      file_name_pl1.append(".csv");
      sample_pl1_file.open (file_name_pl1.c_str());


    /*while (true) {
        cout << "waiting for input" << endl;
        Bottle *input = port.read();
        if (input!=NULL) {
            cout << "got " << input->toString().c_str() << endl;
            double total = 0;
            for (int i=0; i<input->size(); i++) {
                total += input->get(i).asDouble();
            }
            Bottle& output = port.prepare();
            output.clear();
            output.addString("total");
            output.addDouble(total);
            cout << "writing " << output.toString().c_str() << endl;
            port.write();
        }
    }
    return 0;*/

      while(1)
      {
          ts_pl1 = inlet_pl1.pull_sample(&sample_pl1,1,0.009);//wait up to 9ms

          //cout << "got: " << sample_pl1 << endl;


          ReplaceStringInPlace(sample_pl1, "0:", "'0':");
          ReplaceStringInPlace(sample_pl1, "1:", "'1':");
          std::replace( sample_pl1.begin(), sample_pl1.end(), '\'', '"'); // replace all 'x' to 'y'

          sample_pl1_file << sample_pl1 << endl;
            //cout << "sample:" << sample_pl1 << ":end";
          if(sample_pl1.length()>1)//if within 9ms data from glasses arrived
          {

              //json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
              j_obj = json::parse(sample_pl1);

              /*if(sample_pl1.length()>1)
              {
                  found = sample_pl1.find("'timestamp': ");
                  foundtse0 = sample_pl1.find("'timestamp': ",found+1)+12;
                  foundcfe0 = sample_pl1.find(",",foundtse0+1)-1;

                  foundtse1 = sample_pl1.find("'timestamp': ",foundcfe0+1)+12;
                  foundcfe1 = sample_pl1.find(", 'projected_sphere':",foundtse1+1)-1;

                  foundtse2 = sample_pl1.find("'timestamp': ",foundcfe1+1)+12;
                  foundcfe2 = sample_pl1.find(", 'projected_sphere':",foundtse2+1)-1;


                  if(foundtse0<foundcfe0 && foundtse1<foundcfe1)
                  {
                    strtse01 = sample_pl1.substr(foundtse0+1,13);
                    strtse11 = sample_pl1.substr(foundtse1+1,13);
                  }
              }*/
              //json_np = j_obj["timestamp"];//[0]["norm_pos"];
              //json_ts = j_obj;//[0]["timestamp"];

              //cout << "json " << j_obj["timestamp"]  << " " << j_obj["norm_pos"][0] << " " << j_obj["norm_pos"][1] << "endjson";

              pl1_new_data_ready = 1;
              output = port.prepare();
              output.clear();
              output.addDouble(j_obj["timestamp"].get<double>());
              output.addDouble(j_obj["norm_pos"][0].get<double>());
              output.addDouble(j_obj["norm_pos"][1].get<double>());

              sample[0] = (float)(j_obj["timestamp"].get<double>());
              sample[1] = (float)(j_obj["norm_pos"][0].get<double>());
              sample[2] = (float)(j_obj["norm_pos"][1].get<double>());
              outlet.push_sample(sample);
//              output.addString(j_obj);
//              cout << "writing " << output.toString().c_str() << endl;
              port.write();
          }
      }
      sample_pl1_file.close();

  #else
    while(1)
        pl1_new_data_ready = 1;
  #endif
}

void pl2_capture()
{
  #ifdef LSL_PL2
      double ts_pl2;
      ofstream sample_pl_file2;
      string file_name_pl2;
      char input_str2[10000];
      double ts_e02,ts_e12;
      std::size_t found2, foundtse02, foundtse12, foundcfe02, foundcfe12;

      sample_pl_file2.precision(17);

      vector<lsl::stream_info> results = lsl::resolve_stream("name","Gaze Python Representation");

      lsl::stream_inlet inlet_pl2(results[1]);

      file_name_pl2 = "samples_pl2";
      file_name_pl2.append(ctime (&rawtime));
      file_name_pl2.append(".csv");
      sample_pl_file2.open (file_name_pl2.c_str());


      while(1)
      {
        ts_pl2 = inlet_pl2.pull_sample(&sample_pl2,1,0.008334);
        cout << "got: " << sample_pl2 << endl;
        sample_pl_file2 << sample_pl2 << endl;
        if(sample_pl2.length()>1000)
        {

            found2 = sample_pl2.find("'timestamp': ");

            foundtse02 = sample_pl2.find("'timestamp': ",found2+1)+12;
            foundcfe02 = sample_pl2.find(", 'projected_sphere':",foundtse02+1)-1;

            foundtse12 = sample_pl2.find("'timestamp': ",foundcfe02+1)+12;
            foundcfe12 = sample_pl2.find(", 'projected_sphere':",foundtse12+1)-1;


            if(foundtse02<foundcfe02 && foundtse12<foundcfe12)
            {
                strtse02 = sample_pl2.substr(foundtse02+1,13);
                strtse12 = sample_pl2.substr(foundtse12+1,13);
            }

        }
        pl2_new_data_ready = 1;
      }

      sample_pl2_file.close();
  #else
  while(1)
    pl2_new_data_ready = 1;
  #endif


}


int main(int argc, char *argv[])
{

    std::vector<std::thread> threads;

    cout.precision(17);

    cout << "Here is a one-shot resolve of all current streams:" << endl;

	// discover all streams on the network
	vector<lsl::stream_info> results_all = lsl::resolve_streams();

	// display them
	cout << "Display all resolved streams: " << endl;

	for (unsigned k=0;k<results_all.size();k++)
        cout << results_all[k].as_xml() << endl << endl;

    cout << "Now creating the inlet..." << endl;

    time(&rawtime);

    ofstream sample_file;
    string file_name;

    sample_file.precision(17);

	file_name = "samples_";
	file_name.append(ctime (&rawtime));
    file_name.append(".csv");
    sample_file.open(file_name.c_str());

    threads.push_back(std::thread(optitrack_capture));
    threads.push_back(std::thread(pl1_capture));
    threads.push_back(std::thread(pl2_capture));

    sample_file <<"header";
    sample_file <<strtse11;
    sample_file <<",";
    sample_file <<strtse02;
    sample_file <<",";
    sample_file <<strtse12;
    sample_file << endl;
    long int sample = 0;
    while(1)
    {
        time(&rawtime);
        if(ot_new_data_ready && pl1_new_data_ready && pl2_new_data_ready)
        {
            ot_new_data_ready = 0;
            pl1_new_data_ready = 0;
            pl2_new_data_ready = 0;

            for (unsigned c=0;c<11;c++)
            {
                //cout << "\t" << sample_ot[c];

                sample_file << sample_ot[c];
                sample_file <<",";
            }
            cout << ++sample << endl;

            sample_file <<strtse01;
            sample_file <<",";
            sample_file <<strtse11;
            sample_file <<",";
            sample_file <<strtse02;
            sample_file <<",";
            sample_file <<strtse12;
            sample_file << endl;
        }
    }
    sample_file.close();
    for(auto& thread : threads){
        thread.join();
    }



    return 0;

}




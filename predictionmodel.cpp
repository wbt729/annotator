#include "predictionmodel.h"

PredictionModel::PredictionModel(QObject *parent) : QObject(parent)
{
    tensorflow::SessionOptions session_options;
    tensorflow::RunOptions run_options;
    tensorflow::SavedModelBundle bundle;
    session_options.config.mutable_gpu_options()->set_allow_growth(true);
    std::string path = "/home/georg/gles/projekte/dhl/work/object_detection/model/saved_model/";
    auto status = tensorflow::LoadSavedModel(session_options, run_options, path, {"serve"},
            &bundle);

    if (status.ok()){
        std::cout<<"Model loaded successfully"<< std::endl;
    }
    else {
        std::cout<<"Error in loading "<< std::endl;
    }
}

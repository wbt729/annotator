#ifndef PREDICTIONMODEL_H
#define PREDICTIONMODEL_H

#include <QObject>

#include "tensorflow/cc/saved_model/loader.h"
//#include "tensorflow/cc/ops/const_op.h"
//#include "tensorflow/cc/ops/image_ops.h"
//#include "tensorflow/cc/ops/standard_ops.h"
//#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
//#include "tensorflow/core/graph/default_device.h"
//#include "tensorflow/core/graph/graph_def_builder.h"
//#include "tensorflow/core/lib/core/errors.h"
//#include "tensorflow/core/lib/core/stringpiece.h"
//#include "tensorflow/core/lib/core/threadpool.h"
//#include "tensorflow/core/lib/io/path.h"
//#include "tensorflow/core/lib/strings/stringprintf.h"
//#include "tensorflow/core/platform/init_main.h"
//#include "tensorflow/core/platform/logging.h"
//#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
//#include "tensorflow/cc/client/client_session.h"
//#include "tensorflow/core/util/command_line_flags.h"
//#include "tensorflow/core/framework/tensor_slice.h"

class PredictionModel : public QObject
{
    Q_OBJECT
public:
    explicit PredictionModel(QObject *parent = nullptr);


};

#endif // PREDICTIONMODEL_H

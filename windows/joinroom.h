#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>

#include "codec.h"

namespace Ui {
class JoinRoom;
}

enum class DialogType : char {kMaunalCreateRoom, kSearchRoom};

class JoinRoom : public QDialog {
    Q_OBJECT

  public:
    explicit JoinRoom(DialogType dialog_type, QWidget *parent = nullptr);
    ~JoinRoom();
    
    void search_room();
    void join_room();

  private:
    void encode_message(RequestCode reqcode);
    
    Ui::JoinRoom *ui;
};

#endif  // JOINROOM_H

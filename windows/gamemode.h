#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QDialog>

namespace Ui {
class GameMode;
}

class GameMode : public QDialog {
    Q_OBJECT

  public:
    explicit GameMode(QWidget *parent = nullptr);
    ~GameMode();
    
  protected:
    void closeEvent(QCloseEvent* event) override;

  private:
    Ui::GameMode *ui;
};

#endif  // GAMEMODE_H

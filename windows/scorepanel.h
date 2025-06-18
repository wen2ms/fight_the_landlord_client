#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget {
    Q_OBJECT

  public:
    enum FontColor {kBlack, kWhite, kRed, kBlue, kGreen};
    
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();
    
    void set_scores(int left_score, int right_score, int my_score);
    
    void set_score_font_size(int point);
    
    void set_score_font_color(FontColor color);

  private:
    Ui::ScorePanel *ui;
    
    QVector<QLabel*> labels_list_;
};

#endif  // SCOREPANEL_H

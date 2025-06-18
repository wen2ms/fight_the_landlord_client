#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

#include "card.h"
#include "player.h"

class CardPanel : public QWidget {
    Q_OBJECT
  public:
    explicit CardPanel(QWidget *parent = nullptr);
    
    void set_image(const QPixmap& front, const QPixmap& back);
    QPixmap get_image() const;
    
    void set_front_side(bool is_front);
    bool is_front_side() const;
    
    void set_selected_side(bool is_selected);
    bool is_selected() const;
    
    void set_card(const Card& card);
    Card get_card() const;
    
    void set_owner(Player* player);
    Player* get_owner();
    
    void clicked();
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
    void mousePressEvent(QMouseEvent* event) override;

  signals:
    void card_selected(Qt::MouseButton button);
    
  private:
    QPixmap front_;
    QPixmap back_;
    
    bool is_front_;
    bool is_selected_;
    
    Card card_;
    Player* player_;
};

#endif  // CARDPANEL_H

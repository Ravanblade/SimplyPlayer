#include "spectrumanalyzer.h"
#include "simplydefinitions.h"


SpectrumAnalyzer::SpectrumAnalyzer(QWidget *parent) :
    QWidget(parent)
{
    this->setAutoFillBackground(false);
    licznik = 1;
    LoadINI();
    timer = new QTimer(this);
    timer2 = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(Timer2Event()));
    animation = 0;

    if(!timer->isActive())
        timer->start(updateSpeed);

    if(!timer2->isActive())
        timer2->start(updateSpeed*4);



}

void SpectrumAnalyzer::setPlayer(SimplyMediaPlayer *pl)
{
    Player = pl;
}

void SpectrumAnalyzer::TimerEvent()
{
    if(Player->GetState() == 1 || animation == 1)
        this->repaint();
}

void SpectrumAnalyzer::setAnimationMode(int anim)
{

    //timer->stop();
    //timer2->stop();
    animation = anim;

}


void SpectrumAnalyzer::Timer2Event()
{
    if(Player->GetState() == 1 || animation == 1)
    {

        for(int a = 0; a < 1024; a++)
            last_fft[a] = fft[a];

        if(animation == 1)
        {

            for(int s = 0; s < 1024; s++)
            {
                //QTime time = QTime::currentTime();
                //qsrand((float)time.msec());

               // float randomValue = qrand() % 4.0;

                fft[s] = SimplyFunctions::rand_FloatRange(0.001, 0.6);
                last_fft[s] = fft[s];
            }


        }
        else

            BASS_ChannelGetData(Player->GetChannel(),fft,BASS_DATA_FFT2048); // get the FFT data

        licznik = 2;

        for(int i = 0; i < 1024; i++)
        {
           // qDebug() << fft[i];
            fft[i] = sqrt(fft[i])*5;
            h_fft[i] = last_fft[i];
           // qDebug() << fft[i];
        }

    }
}

void SpectrumAnalyzer::LoadINI()
{
    QSettings s(SPECTRUMCONFIG_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(SPECTRUMCONFIG_PATH);

    s.beginGroup("Spectrum");

  //  int count =
  //  int number = 0;

  //  for(int i = 0; i < count; i++)
  //  {
 //       if(settings.value("Playlist"+QString::number(i), "") == playlist_name)

    gk = s.value("BrickThickness",4).toInt(); // grubosc kratki domyslne: gk = 2 min: 2
    gs = s.value("BarThickness",3).toInt(); // gs = 3 min: 1
    odstepy = s.value("Spacing",4).toInt()+gs; // domyslne 4 min: 1
    grid = s.value("Grid",false).toBool();
    kostka = s.value("Bricks",false).toBool();
    reflect = s.value("Reflections",false).toBool();
    mirror = s.value("Mirror",false).toBool();
    waves = s.value("Waves",false).toBool();
    upToDown = s.value("FromUpToDown",false).toBool();
    numOfGradients = s.value("GradientsNumber",2).toInt();
    gradient_1 = QColor(s.value("Gradient1_R", 255).toInt(),s.value("Gradient1_G", 255).toInt(),s.value("Gradient1_B", 255).toInt(),s.value("Gradient1_A", 255).toInt());
    gradient_2 = QColor(s.value("Gradient2_R", 255).toInt(),s.value("Gradient2_G", 255).toInt(),s.value("Gradient2_B", 255).toInt(),s.value("Gradient2_A", 255).toInt());
    kolor_tla = QColor(s.value("Background_R", 255).toInt(),s.value("Background_G", 255).toInt(),s.value("Background_B", 255).toInt(),s.value("Background_A", 255).toInt());
    bar_count = (this->width()/odstepy+odstepy)*(odstepy*2);
    //qDebug() << bar_count;
    widgetMask = QPixmap(this->width()*2+40, this->height()*2+20);

    dropSpeed = s.value("DropSpeed",0.02).toFloat();
    updateSpeed = s.value("UpdateSpeed",27).toFloat();
    growSpeed = s.value("GrowSpeed",0.05).toFloat();

    s.endGroup();


}

void SpectrumAnalyzer::LoadCustomSettings(SpectrumSettings set)
{
    gk = set.gk;
    gs = set.gs;
    odstepy = set.odstepy;
    grid = set.grid;
    kostka = set.kostka;
    reflect = set.reflect;
    mirror = set.mirror;
    waves = set.waves;
    upToDown = set.upToDown;
    gradient_1 = set.gradient_1;
    gradient_2 = set.gradient_2;
    kolor_tla = set.kolor_tla;

    dropSpeed = set.dropSpeed;
    updateSpeed = set.updateSpeed;
    growSpeed = set.growSpeed;

    if(timer->isActive())
    {
        timer->stop();
        timer2->stop();
    }

    timer->start(updateSpeed);
    timer2->start(updateSpeed*4);

}



void SpectrumAnalyzer::paintEvent(QPaintEvent *event)
{

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, 35));
    linearGrad.setSpread(QGradient::PadSpread);
    linearGrad.setColorAt(1, gradient_1);
    linearGrad.setColorAt(0, gradient_2);


//    float gk = 2, gs = 1; // grubosc kratki domyslne: gk = 2 min: 2 , gs = 3 min: 1
//    float odstepy = 2; // domyslne 4 min: 1
//    float h_gk = 6;
//    bool grid = true;
//    bool kostka = true;
//    bool reflect = true;
//    bool mirror = false;
//    bool upToDown = false;
//    QColor gradient_1 = Qt::green;
//    QColor gradient_2 = Qt::green;
//    QColor kolor_tla = Qt::black;
//    //QColor linearGrad = Qt::green;





      widgetMask.fill(kolor_tla);
      p.begin(&widgetMask);





          QRect background = QRect(0,0,this->width()+0,this->height()+0);
          p.fillRect(background,Qt::black);
          p.drawRect(background);

      p.setBrush(Qt::NoBrush);
      p.setPen(Qt::NoPen);




      if(mirror) // Odbicie rownomierne
      {


          for(int i = 0; i < bar_count; i+=odstepy)
          {

              if(grid)
              {
                  QRect grd = QRect(i,30,gs,50*-1);
                  p.setOpacity(0.2f);
                  linearGrad.setColorAt(0, gradient_1);
                  linearGrad.setColorAt(1, gradient_2);
                  p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                  p.drawRect(grd);
                  p.setOpacity(1);
              }


             QRect col, peak;

             if((fft[i] > last_fft[i]) && (fft[i] > 0))
             {

                 if(last_fft[i]*30 > 15)
                 {
                     last_fft[i] += growSpeed;
                     col = QRect(i,20,gs,last_fft[i]*20*-1);
                 }
                 else
                 {
                     last_fft[i] += growSpeed;
                     col = QRect(i,20,gs,last_fft[i]*30*-1);
                 }

            }
             else
             {


                  if(fft[i] <= 0)
                      fft[i] = 0;


                  if(fft[i]*30 > 15)
                  {
                      fft[i] -= dropSpeed;
                      col = QRect(i,20,gs,fft[i]*20*-1);
                  }
                  else
                  {
                      fft[i] -= dropSpeed;
                      col = QRect(i,20,gs,fft[i]*30*-1);
                  }

             }

             p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
             p.drawRect(col);
          }

          for(int i = 0; i < bar_count; i+=odstepy)
          {

              if(grid)
              {
                  QRect grd = QRect(i,30,gs,50);
                  p.setOpacity(0.2f);
                  linearGrad.setColorAt(0, gradient_1);
                  linearGrad.setColorAt(1, gradient_2);
                  p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                  p.drawRect(grd);
                  p.setOpacity(1);
              }

              QRect col, peak;

              if((fft[i] > last_fft[i]) && (fft[i] > 0))
              {

                  if(last_fft[i]*30 > 15)
                  {
                      last_fft[i] += growSpeed;
                      col = QRect(i,20,gs,last_fft[i]*20);
                  }
                  else
                  {

                    last_fft[i] += growSpeed;
                    col = QRect(i,20,gs,last_fft[i]*30);
                  }

             }
              else
              {


                   if(fft[i] <= 0)
                       fft[i] = 0;


                   if(fft[i]*30 > 15)
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,20,gs,fft[i]*20);
                   }
                   else
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,20,gs,fft[i]*30);
                   }

              }


             p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
             p.drawRect(col);
          }





      }
      else if(reflect) // Odbicie na lustrze wody
      {
          int i = 0;

          for(i = 0; i < bar_count; i+=odstepy)
          {
             //qDebug() << sqrt(fft[10+i+22])*300;
             //QRect col;

             int alg = i-gk+1;
             if(alg < 0)
                 alg = 1;

             QRect col, peak;

             if((fft[i] > last_fft[i]) && (fft[i] > 0))
             {

                 if(last_fft[i]*30 > 20)
                 {
                     last_fft[i] += growSpeed; //0.025
                     col = QRect(i,30,gs,last_fft[i]*20*-1);
                 }
                 else
                 {
                     last_fft[i] += growSpeed;
                     col = QRect(i,30,gs,last_fft[i]*30*-1);
                 }

            }
             else
             {


                  if(fft[i] <= 0)
                      fft[i] = 0;


                  if(fft[i]*30 > 20)
                  {
                      fft[i] -= dropSpeed;
                      col = QRect(i,30,gs,fft[i]*20*-1);
                  }
                  else
                  {
                      fft[i] -= dropSpeed;
                      col = QRect(i,30,gs,fft[i]*30*-1);
                  }

             }


             if(grid)
             {
                 QRect grd = QRect(i,30,gs,60*-1);
                 p.setOpacity(0.2f);
                 p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                 p.drawRect(grd);
                 p.setOpacity(1);
             }

             p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
             p.drawRect(col);
          }

          p.setOpacity(0.4f);
          i = 0;


          for(i = 0; i < bar_count; i+=odstepy)
          {

              QRect col, peak;

              if((fft[i] > last_fft[i]) && (fft[i] > 0))
              {

                  if(last_fft[i]*30 > 15)
                  {
                      last_fft[i] += growSpeed;
                      col = QRect(i,30,gs,last_fft[i]*20);
                  }
                  else
                  {

                    last_fft[i] += growSpeed;
                    col = QRect(i,30,gs,last_fft[i]*30);
                  }

             }
              else
              {


                   if(fft[i] <= 0)
                       fft[i] = 0;


                   if(fft[i]*30 > 15)
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,30,gs,fft[i]*20);
                   }
                   else
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,30,gs,fft[i]*30);
                   }

              }



             if(grid)
             {
                 QRect grd = QRect(i,30,gs,50);
                 p.setOpacity(0.2f);
                 linearGrad.setColorAt(0, gradient_1);
                 linearGrad.setColorAt(1, gradient_2);
                 p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                 p.drawRect(grd);
                 p.setOpacity(0.4f);
             }

             p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
             p.drawRect(col);
          }
          p.setOpacity(1);
      }

      else if(upToDown)
      {


          for(int i = 0; i < bar_count; i+=odstepy)
          {
             // qDebug() << sqrt(fft[10+i+22])*300;
              QRect col, peak;



              if((fft[i] > last_fft[i]) && (fft[i] > 0))
              {

                  if(last_fft[i]*40 > 25)
                  {
                      last_fft[i] += growSpeed;
                      col = QRect(i,0,gs,last_fft[i]*30);
                  }
                  else
                  {

                    last_fft[i] += growSpeed;
                    col = QRect(i,0,gs,last_fft[i]*40);
                  }

             }

              else
              {

                   if(fft[i] <= 0)
                       fft[i] = 0;


                   if(fft[i]*40 > 25)
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,0,gs,fft[i]*30);
                   }
                   else
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,0,gs,fft[i]*40);
                   }

              }


              if(grid)
              {
                  QRect grd = QRect(i,0,gs,80);
                  p.setOpacity(0.2f);
                  linearGrad.setColorAt(1, gradient_1);
                  linearGrad.setColorAt(0, gradient_2);
                  p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                  p.drawRect(grd);
                  //p.drawChord();

                //  painter.drawPolygon(points, 4);

                  p.setOpacity(1);
              }



              p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
              p.drawRect(col);
          }

      }
      else if(waves)
      {


          for(int i = 0; i < bar_count; i+=odstepy)
          {
             // qDebug() << sqrt(fft[10+i+22])*300;
              QRect col, peak;



              if((fft[i] > last_fft[i]) && (fft[i] > 0))
              {

                  if(last_fft[i]*40 > 25)
                  {
                      last_fft[i] += growSpeed;
                      col = QRect(i,40,gs,last_fft[i]*30*-1);
                  }
                  else
                  {

                    last_fft[i] += growSpeed;
                    col = QRect(i,40,gs,last_fft[i]*40*-1);
                  }

             }

              else
              {

                   if(fft[i] <= 0)
                       fft[i] = 0;


                   if(fft[i]*40 > 25)
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,40,gs,fft[i]*30*-1);
                   }
                   else
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,40,gs,fft[i]*40*-1);
                   }

              }


              if(grid)
              {
                  QRect grd = QRect(i,0,gs,80);
                  p.setOpacity(0.2f);
                  linearGrad.setColorAt(1, gradient_1);
                  linearGrad.setColorAt(0, gradient_2);
                  p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                  p.drawRect(grd);
                  p.setOpacity(1);
              }



              p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
              p.drawRect(col);
          }





      }


      else
      {


          for(int i = 0; i < bar_count; i+=odstepy)
          {
             // qDebug() << sqrt(fft[10+i+22])*300;
              QRect col, peak;



              if((fft[i] > last_fft[i]) && (fft[i] > 0))
              {

                  if(last_fft[i]*40 > 25)
                  {
                      last_fft[i] += growSpeed;
                      col = QRect(i,40,gs,last_fft[i]*30*-1);
                  }
                  else
                  {

                    last_fft[i] += growSpeed;
                    col = QRect(i,40,gs,last_fft[i]*40*-1);
                  }

             }

              else
              {

                   if(fft[i] <= 0)
                       fft[i] = 0;


                   if(fft[i]*40 > 25)
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,40,gs,fft[i]*30*-1);
                   }
                   else
                   {
                       fft[i] -= dropSpeed;
                       col = QRect(i,40,gs,fft[i]*40*-1);
                   }

              }


              if(grid)
              {
                  QRect grd = QRect(i,0,gs,80);
                  p.setOpacity(0.2f);
                  linearGrad.setColorAt(1, gradient_1);
                  linearGrad.setColorAt(0, gradient_2);
                  p.fillRect(grd,linearGrad); //QColor(128, 128, 255, 128));
                  p.drawRect(grd);
                  p.setOpacity(1);
              }



              p.fillRect(col,linearGrad); //QColor(128, 128, 255, 128));
              p.drawRect(col);
          }

      }
      // p.setPen(Qt::black);

       if(kostka || grid)
       {

           //Poprzeczne linie tworzace efekt kostek
           for(int j = 0; j < this->height(); j+=gk)
           {
               //p.drawLine(0,2+j*7,220,2+j*7);
               QRect line = QRect(0,j,250,1);
               p.fillRect(line,kolor_tla); //QColor(128, 128, 255, 128));
               p.drawRect(line);
               //p.drawLine(0,2+j*3,0,2+j*3+30);
           }
       }

       //grid
       if(grid)
       {

           for(int j = odstepy; j < bar_count; j+=odstepy)
           {
               //p.drawLine(0,2+j*7,220,2+j*7);
               QRect line = QRect(j-1,0,1,40);
               p.fillRect(line,kolor_tla); //QColor(128, 128, 255, 128));
               p.drawRect(line);
               //p.drawLine(0,2+j*3,0,2+j*3+30);
           }
       }

     // p.drawLine(QLine(30,30,30,sqrt(fft[300])*20));
      //p.drawRect(20,2,3,sqrt(fft[300])*1000);

      p.end();


      p.begin(this);
      p.setBackgroundMode(Qt::OpaqueMode);
      p.setRenderHint(QPainter::HighQualityAntialiasing);
      p.setCompositionMode(QPainter::CompositionMode_Source);
      p.fillRect(rect(), Qt::black);
      p.drawPixmap(0,0,widgetMask);
      //p.drawPixmap();
      p.end();



   // QWidget::paintEvent(event);
}

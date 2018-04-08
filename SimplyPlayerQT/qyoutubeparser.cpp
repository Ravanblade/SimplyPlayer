#include "qyoutubeparser.h"


QStringList QYouTubeParser::getYouTubeVideo( const QString &data)
{
    QList< int > unsupportedSignatureLengths;
    QStringList ret;
    for ( int i = 0 ; i <= 1 ; ++i )
    {
        qDebug() << "1";
        int streamsIdx = data.indexOf( QString( i ? "adaptive_fmts" : "url_encoded_fmt_stream_map" ) + "\": \"" ); //"adaptive_fmts" contains audio or video urls
        if ( streamsIdx > -1 )
        {
            qDebug() << "2";
            streamsIdx += 30;
            int streamsEndIdx = data.indexOf( '"', streamsIdx );
            if ( streamsEndIdx > -1 )
            {
                qDebug() << "3";
                foreach ( QString stream, data.mid( streamsIdx, streamsEndIdx - streamsIdx ).split( ',' ) )
                {
                    int itag = -1;
                    QString ITAG, URL, Signature;
                    foreach ( QString streamParams, stream.split( "\\u0026" ) )
                    {
                        qDebug() << "4";
                        const QStringList paramL = streamParams.split( '=' );
                        if ( paramL.count() == 2 )
                        {
                            if ( paramL[ 0 ] == "itag" )
                            {
                                ITAG = "itag=" + paramL[ 1 ];
                                itag = paramL[ 1 ].toInt();
                            }
                            else if ( paramL[ 0 ] == "url" )
                                URL = unescape( paramL[ 1 ] );
                            //else if ( paramL[ 0 ] == "sig" )
                             //   Signature = paramL[ 1 ];
                            else if ( paramL[ 0 ] == "s" )
                            {
                                const QString s = paramL[ 1 ];
                                bool found = false;

                                foreach ( QString decodeScript, decodeScripts.split( '\n', QString::SkipEmptyParts ) )
                                {
                                    const QStringList operations = decodeScript.split( ',', QString::SkipEmptyParts );
                                    if ( !operations.isEmpty() && operations[ 0 ].toInt() == s.length() )
                                    {
                                        for ( int i = 1 ; i < operations.count() ; ++i )
                                        {
                                            const QStringList operationList = operations[ i ].split( ':' );
                                            if ( operationList.count() == 3 )
                                            {
                                                if ( operationList[ 0 ] == "r" )
                                                    Signature += Reverse( s, operationList[ 1 ].toInt(), operationList[ 2 ].toInt() );
                                                else if ( operationList[ 0 ] == "m" )
                                                    Signature += s.mid( operationList[ 1 ].toInt(), operationList[ 2 ].toInt() );
                                            }
                                            else if ( operationList.count() == 1 )
                                            {
                                                bool ok;
                                                const int idx = operationList[ 0 ].toInt( &ok );
                                                if ( ok && idx >= 0 && idx < s.length() )
                                                    Signature += s[ idx ];
                                            }
                                        }
                                        found = true;
                                        break;
                                    }
                                }
                                if ( !found && !unsupportedSignatureLengths.contains( s.length() ) )
                                {

                                    unsupportedSignatureLengths += s.length();
                                }
                            }
                        }
                    }
                   // qDebug() << URL << ITAG << itag;
                   // if ( !URL.isEmpty() && ( !Signature.isEmpty() || URL.contains( "signature" ) ) )
                    //{


                        if ( !Signature.isEmpty() )
                            URL += "&signature=" + Signature;

                      //  if(validateItag(QString::number(itag))) //Tylko mp4
                     //   {
                             YOUTUBE_FILE file;
                             file.fileURL = URL;
                             //file.signature = 0;
                             file.itag = itag;
                             file.quality = itagToQuality(QString::number(itag));
                             files.append(file);


                      //   }

                        qDebug() << URL << ITAG << itag << Signature;
                    //}
                }
            }
        }
    }

    if ( ret.count() == 2 ) //Pobiera tytuł
    {
        int ytplayerIdx = data.indexOf( "ytplayer.config" );
        if ( ytplayerIdx > -1 )
        {
            int titleIdx = data.indexOf( "title\": \"", ytplayerIdx );
            if ( titleIdx > -1 )
            {
                int titleEndIdx = data.indexOf( '"', titleIdx += 9 );
                if ( titleEndIdx > -1 )
                    title = fromU( data.mid( titleIdx, titleEndIdx - titleIdx ));
            }
        }
        if ( ret.count() == 2 )
            ret << "Can't find title";
    }
    emit isParsed();
    return ret;
}

QString QYouTubeParser::unescape( const QString &str )
{
    return QByteArray::fromPercentEncoding( str.toLatin1() );
}

QString QYouTubeParser::fromU( QString s )
{
    int uIdx = 0;
    for ( ;; )
    {
        uIdx = s.indexOf( "\\u", uIdx );
        if ( uIdx == -1 )
            break;
        bool ok;
        const QChar chr = s.mid( uIdx + 2, 4 ).toUShort( &ok, 16 );
        if ( ok )
        {
            s.replace( uIdx, 6, chr );
            ++uIdx;
        }
        else
            uIdx += 6;
    }
    return s;
}

QString QYouTubeParser::Reverse( const QString &str, int from, int to )
{
    QString ret;
    const int count = qAbs( to - from );
    for ( int i = 0, s = from ; i < count ; ++i, --s )
        ret += str[ s ];
    return ret;
}


QYouTubeParser::QYouTubeParser(QString link)
{
    parserTimer = new QTimer(this);
    connect(parserTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    parserTimer->start(5);
    downloadPageData(link);


}

QYouTubeParser::QYouTubeParser()
{

}

void QYouTubeParser::parseLink(QString link)
{

}

void QYouTubeParser::timerEvent()
{
    QFileInfo pathInfo(APPLICATION_PATH + "parsed_data.txt");

    if(pathInfo.size() > 0)
    {
       parserTimer->stop();

       QFile qss(APPLICATION_PATH + "parsed_data.txt");
       qss.open(QFile::ReadOnly);
       pageContentV = qss.readAll();
       qss.close();
       qDebug() <<"sprasowane";

       isDownloaded();

       emit isParsed();
    }
    counter++;

    if(counter > 5000) //timeout
        parserTimer->stop();

}

YOUTUBE_SEARCH QYouTubeParser::extractSearchedLinks()
{

  /*    '
    <media:title type='plain'> </media:title>
    <yt:duration seconds='255            '
    */



        YOUTUBE_SEARCH srch;
        QStringList list;
        QStringList title;
        QStringList time;
        QString content = pageContentV;
        QString titles = pageContentV;
        QString times = pageContentV;


        while(1)
        {
            int pos = content.indexOf("<media:content url='");

            if(pos < 0)
                break;

            content.remove(0, pos+20);

            pos = content.indexOf("'",0);

            if(pos < 0)
                break;

            QString tmp = content;

            tmp.remove( pos, tmp.size() - pos );


            if(!SimplyFunctions::isRTSP(tmp))
            {

                int pos3 = tmp.indexOf("/v/",0);
                tmp.remove(0,pos3+3);
                int pos2 = tmp.indexOf("?",0);
                tmp.remove(pos2,tmp.size());

                if(!list.contains("http://www.youtube.com/watch?v=" + tmp))
                    list.append("http://www.youtube.com/watch?v=" + tmp);

            }


            content.remove(0, pos);
        }

        while(1)
        {
            int pos = titles.indexOf("<media:title type='plain'>");

            if(pos < 0)
                break;

            titles.remove(0, pos+26);

            pos = titles.indexOf("</media:title>",0);

            if(pos < 0)
                break;

            QString tmp = titles;

            tmp.remove( pos, tmp.size() - pos );

            title.append(tmp);

            titles.remove(0, pos);
        }

        while(1)
        {
            int pos = times.indexOf("<yt:duration seconds='");

            if(pos < 0)
                break;

            times.remove(0, pos+22);

            pos = times.indexOf("'",0);

            if(pos < 0)
                break;

            QString tmp = times;

            tmp.remove( pos, tmp.size() - pos );

            time.append(tmp);

            times.remove(0, pos);
        }










 /*
        int posy = content.indexOf("id=\"search-results\"");
        content.remove(0,posy);

        posy = titles.indexOf("id=\"search-results\"");
        titles.remove(0,posy);

        posy = content.indexOf("</div></div></ol></div></div>");
        content.remove(posy,content.size());

        posy = titles.indexOf("</div></div></ol></div></div>");
        titles.remove(posy,content.size());




         //usuniecie buttonow
         while(1)
         {
              int pos = content.indexOf("<button");
              if(pos < 0)
              {
                 // qDebug() << "dupoa";
                  break;
              }

              int pos2 = content.indexOf("</button>",pos);
              content.remove(pos,pos2-pos);
              //qDebug() << pos2-pos;


         }


         //usuniecie meta
         while(1)
         {
              int pos = content.indexOf("<div class=\"yt-lockup-meta\">");
              if(pos < 0)
              {
                 // qDebug() << "dupoa";
                  break;
              }

              int pos2 = content.indexOf("</div>",pos);
              content.remove(pos,pos2-pos);



         }

        while(1)
        {
            int pos = content.indexOf("href=\"/watch?v");

            if(pos < 0)
                break;

            content.remove(0, pos+6);

            pos = content.indexOf("\"",8);
            //qDebug() << pos;
            if(pos < 0)
                break;

            int pos2 = content.indexOf("&amp",8);

            QString tmp = content;

            if(pos2 > pos)
                tmp.remove( pos, tmp.size() - pos );
            else
                tmp.remove( pos2, tmp.size() - pos2 );


            if(!list.contains("http://www.youtube.com" + tmp))
                list.append("http://www.youtube.com" + tmp);

            //  qDebug() << list;

            content.remove(0, pos2 > pos ? pos : pos2);
        }




        //usuniecie buttonow
        while(1)
        {
             int pos = titles.indexOf("<button");
             if(pos < 0)
             {

                 break;
             }

             int pos2 = titles.indexOf("</button>",pos);
             titles.remove(pos,pos2-pos);
            // qDebug() << pos2-pos;


        }


        //usuniecie meta
        while(1)
        {
             int pos = titles.indexOf("<div class=\"yt-lockup-meta\">");
             if(pos < 0)
             {

                 break;
             }

             int pos2 = titles.indexOf("</div>",pos);
             titles.remove(pos,pos2-pos);
             //qDebug() << pos2-pos;


        }


        while(1)
        {
            int pos = titles.indexOf("dir=\"ltr\"");

            if(pos < 0)
                break;

            titles.remove(0, pos+9);

            pos = titles.indexOf("title=\"");

            int pos2 = titles.indexOf("dir=\"ltr\"");

            if(pos2 < pos)
            {
                titles.remove(0, pos2-1);
                continue;
            }

            titles.remove(0, pos+7);

            pos = titles.indexOf("\"",0);

            if(pos < 0)
                break;

            QString tmp = titles;

            tmp.remove( pos, tmp.size() - pos );

            if(tmp != "__title__" && title.count() != list.count())
                title.append(tmp);

            //  qDebug() << list;

            titles.remove(0, pos);
        }
*/
        srch.title = title;
        srch.url = list;
        srch.time = time;

       // qDebug() << srch.title;
        return srch;

}



void QYouTubeParser::downloadPageData(QString link)
{

    //QString program = APPLICATION_PATH + "";
    QStringList arguments;
    arguments << link;

    //QProcess *myProcess = new QProcess();
    //myProcess->start(program, arguments);
    QString fileName = QDir::fromNativeSeparators( QApplication::applicationDirPath() + "//parsed_data.txt");
    QFile file(fileName);
    file.remove();

   // QProcess process;
   // QString file = "LinkParser.exe";
   // process.start(file,arguments);
    QString bytes = QString(APPLICATION_PATH + "LinkParser.exe");

    //char *filename = bytes.data();
    //system(filename);
    proc = new QProcess();
    proc->start(bytes,arguments);


    parserTimer->start();
    proc->waitForFinished(3000);


   // qDebug() << proc->arguments();

}

void QYouTubeParser::isDownloaded()
{
   // qDebug() << pageContentV;
    decodeScripts = pageContentV;
   // getYouTubeVideo(pageContentV);
   // parseData();
    //emit parsed(this);
}

bool QYouTubeParser::validateLink(QString link)
{

 QString content = link;

 return (content.size() > 20);

}

bool QYouTubeParser::validateItag(QString itag)
{

        int iTagValue = itag.toInt();
        if (itag != "")
        {

            switch (iTagValue)
            {
              //  case 5: return true; break; //q.SetQuality("flv", new Size(320, (_Wide ? 180 : 240))); break;
              //  case 6: return true; break;//q.SetQuality("flv", new Size(480, (_Wide ? 270 : 360))); break;
              //  case 17: return true; break;//q.SetQuality("3gp", new Size(176, (_Wide ? 99 : 144))); break;
                case 18: return true; break;//q.SetQuality("mp4", new Size(640, (_Wide ? 360 : 480))); break;
                case 22: return true; break;//q.SetQuality("mp4", new Size(1280, (_Wide ? 720 : 960))); break;
              //  case 34: return true; break;//q.SetQuality("flv", new Size(640, (_Wide ? 360 : 480))); break;
              //  case 35: return true; break;//q.SetQuality("flv", new Size(854, (_Wide ? 480 : 640))); break;
              //  case 36: return true; break;//q.SetQuality("3gp", new Size(320, (_Wide ? 180 : 240))); break;
                case 141: return true; break;//q.SetQuality("mp4", new Size(1920, (_Wide ? 1080 : 1440))); break;
                case 140: return true; break;//q.SetQuality("mp4", new Size(2048, (_Wide ? 1152 : 1536))); break;
                case 139: return true; break;


               // case 43: return false; break;//q.SetQuality("webm", new Size(640, (_Wide ? 360 : 480))); break;
               // case 44: return false; break;//q.SetQuality("webm", new Size(854, (_Wide ? 480 : 640))); break;
               // case 45: return false; break;//q.SetQuality("webm", new Size(1280, (_Wide ? 720 : 960))); break;
               // case 46: return false; break;//q.SetQuality("webm", new Size(1920, (_Wide ? 1080 : 1440))); break;
                /*case 82: return true; break;//q.SetQuality("3D.mp4", new Size(480, (_Wide ? 270 : 360))); break;        // 3D
                case 83: return true; break;//q.SetQuality("3D.mp4", new Size(640, (_Wide ? 360 : 480))); break;        // 3D
                case 84: return true; break;//q.SetQuality("3D.mp4", new Size(1280, (_Wide ? 720 : 960))); break;       // 3D
                case 85: return true; break;//q.SetQuality("3D.mp4", new Size(1920, (_Wide ? 1080 : 1440))); break;     // 3D
                case 100: return true; break;//q.SetQuality("3D.webm", new Size(640, (_Wide ? 360 : 480))); break;      // 3D
                case 101: return true; break;//q.SetQuality("3D.webm", new Size(640, (_Wide ? 360 : 480))); break;      // 3D
                case 102: return true; break;//q.SetQuality("3D.webm", new Size(1280, (_Wide ? 720 : 960))); break;     // 3D
                case 120: return true; break;//q.SetQuality("live.flv", new Size(1280, (_Wide ? 720 : 960))); break;    // Live-streaming - should be ignored?
                */default: return false; break;//q.SetQuality("itag-" + itag, new Size(0, 0)); break;       // unknown or parse error
            }

        }
        else
            return false;
}

QString QYouTubeParser::itagToQuality(QString itag)
{
    int iTagValue = itag.toInt();
    if (itag != "")
    {

        switch (iTagValue)
        {
          //  case 5: return true; break; //q.SetQuality("flv", new Size(320, (_Wide ? 180 : 240))); break;
          //  case 6: return true; break;//q.SetQuality("flv", new Size(480, (_Wide ? 270 : 360))); break;
          //  case 17: return true; break;//q.SetQuality("3gp", new Size(176, (_Wide ? 99 : 144))); break;
            case 18: return "Low"; break;//q.SetQuality("mp4", new Size(640, (_Wide ? 360 : 480))); break;
            case 22: return "Medium"; break;//q.SetQuality("mp4", new Size(1280, (_Wide ? 720 : 960))); break;
          //  case 34: return true; break;//q.SetQuality("flv", new Size(640, (_Wide ? 360 : 480))); break;
          //  case 35: return true; break;//q.SetQuality("flv", new Size(854, (_Wide ? 480 : 640))); break;
          //  case 36: return true; break;//q.SetQuality("3gp", new Size(320, (_Wide ? 180 : 240))); break;
            case 37: return "High"; break;//q.SetQuality("mp4", new Size(1920, (_Wide ? 1080 : 1440))); break;
            case 38: return "Ultra"; break;//q.SetQuality("mp4", new Size(2048, (_Wide ? 1152 : 1536))); break;
           // case 43: return false; break;//q.SetQuality("webm", new Size(640, (_Wide ? 360 : 480))); break;
           // case 44: return false; break;//q.SetQuality("webm", new Size(854, (_Wide ? 480 : 640))); break;
           // case 45: return false; break;//q.SetQuality("webm", new Size(1280, (_Wide ? 720 : 960))); break;
           // case 46: return false; break;//q.SetQuality("webm", new Size(1920, (_Wide ? 1080 : 1440))); break;
            /*case 82: return true; break;//q.SetQuality("3D.mp4", new Size(480, (_Wide ? 270 : 360))); break;        // 3D
            case 83: return true; break;//q.SetQuality("3D.mp4", new Size(640, (_Wide ? 360 : 480))); break;        // 3D
            case 84: return true; break;//q.SetQuality("3D.mp4", new Size(1280, (_Wide ? 720 : 960))); break;       // 3D
            case 85: return true; break;//q.SetQuality("3D.mp4", new Size(1920, (_Wide ? 1080 : 1440))); break;     // 3D
            case 100: return true; break;//q.SetQuality("3D.webm", new Size(640, (_Wide ? 360 : 480))); break;      // 3D
            case 101: return true; break;//q.SetQuality("3D.webm", new Size(640, (_Wide ? 360 : 480))); break;      // 3D
            case 102: return true; break;//q.SetQuality("3D.webm", new Size(1280, (_Wide ? 720 : 960))); break;     // 3D
            case 120: return true; break;//q.SetQuality("live.flv", new Size(1280, (_Wide ? 720 : 960))); break;    // Live-streaming - should be ignored?
            */default: return "Unknown"; break;//q.SetQuality("itag-" + itag, new Size(0, 0)); break;       // unknown or parse error
        }

    }
    else
        return "Unknown";

}



QString QYouTubeParser::decryptSignature(QString sig)
{
      #define sec(s,a,b) SimplyFunctions::section(s,a,b)

     /*OLD signature reader
      * if (sig.size() == 88)
        return sig[48] + sec(sig,82,68) + sig[82] + sec(sig,67,63) + sig[85] + sec(sig,62,49) + sig[67] + sec(sig,48,13) + sig[3] + sec(sig,12,4) + sig[2] + sig[12];
      else if (sig.size() == 87)
        return sig[62] + sec(sig,83,63) + sig[83] + sec(sig,62,53) + sig[0] + sec(sig,52,3);
      else if (sig.size() == 86)
        return sec(sig,2,62) + sig[82] + sec(sig,64,81) + sig[63];
      else if (sig.size() == 85)
        return sig[76] + sec(sig,83,77) + sig[83] + sec(sig,76,61) + sig[0] + sec(sig,60,51) + sig[1] + sec(sig,50,3);
      else if (sig.size() == 84)
        return sec(sig,84,37) + sig[2] + sec(sig,36,27) + sig[3] + sec(sig,26,4) + sig[26];
      else if (sig.size() == 83)
        return sig[52] + sec(sig,82,56) + sig[2] + sec(sig,55,53) + sig[82] + sec(sig,52,37) + sig[55] + sec(sig,36,3) + sig[36];
      else if (sig.size() == 82)
        return sig[36] + sec(sig,80,68) + sig[81] + sec(sig,67,41) + sig[33] + sec(sig,40,37) + sig[40] + sig[35] + sig[0] + sig[67] + sec(sig,33,1) + sig[34];
      */
    qDebug() << sig.size();
    if (sig.size() == 92)
        return sig[25] + sec(sig,3,25) + sig[0] + sec(sig,26,42) + sig[79] + sec(sig,43,79) + sig[91] + sec(sig,80,83);
    else if (sig.size() == 90)
        return sig[25] + sec(sig,3,25) + sig[2] + sec(sig,26,40) + sig[77] + sec(sig,41,77) + sig[89] + sec(sig,78,81);
    else if (sig.size() == 89)
        return sec(sig,85,79) + sig[87] + sec(sig,78,61) + sig[0] + sec(sig,60,4);
    else if (sig.size() == 88)
        return sig[48] + sec(sig,82,68) + sig[82] + sec(sig,67,63) + sig[85] + sec(sig,62,49) + sig[67] + sec(sig,48,13) + sig[3] + sec(sig,12,4) + sig[2] + sig[12];
    else if (sig.size() == 87)
        return sec(sig,84,54) + sig[3] + sec(sig,53,41) + sig[86] + sec(sig,40,11) + sig[0] + sec(sig,10,4) + sig[53];
    else if (sig.size() == 86)
        return sec(sig,5,20) + sig[2] + sec(sig,21,86);
    else if (sig.size() == 85)
        return sec(sig,2,8) + sig[0] + sec(sig,9,21) + sig[65] + sec(sig,22,65) + sig[84] + sec(sig,66,82) + sig[21];
    else if (sig.size() == 84)
        return sec(sig,84,28) + sig[0] + sec(sig,27,6) + sec(sig,3,1) + sig[27];
    else if (sig.size() == 83)
        return sec(sig,0,15) + sig[80] + sec(sig,16,80) + sig[15];
    else if (sig.size() == 82)
        return sig[36] + sec(sig,80,68) + sig[81] + sec(sig,67,41) + sig[33] + sec(sig,40,37) + sig[40] + sig[35] + sig[0] + sig[67] + sec(sig,33,1) + sig[34];
    else if (sig.size() == 81)
        return sig[56] + sec(sig,80,57) + sig[41] + sec(sig,56,42) + sig[80] + sec(sig,41,35) + sig[0] + sec(sig,34,30) + sig[34] + sec(sig,29,10) + sig[29] + sec(sig,9,1) + sig[9];
    else if (sig.size() == 79)
        return sig[54] + sec(sig,78,55) + sig[39] + sec(sig,54,40) + sig[78] + sec(sig,39,35) + sig[0] + sec(sig,34,30) + sig[34] + sec(sig,29,10) + sig[29] + sec(sig,9,1) + sig[9];
    else
        return "Unable to decrypt signature";

}


bool QYouTubeParser::parseData()
{

    QString page_content = pageContentV;
    page_content = QUrl::fromPercentEncoding(page_content.toLatin1());
   // qDebug() << "W dupe?2";
    length = page_content;
    // qDebug() << title;
    int pos = length.indexOf("length_seconds\":");

    if ( pos < 0 )
        length = "Unknown length";

    length.remove( 0, pos+16 );

    pos = length.indexOf( "," );
    if ( pos < 0 )
        length = "Unknown length";

    length.remove( pos, length.size() - pos );

    title = page_content;
   // qDebug() << title;
    pos = title.indexOf("<meta name=\"title\"");
    if ( pos < 0 )
        title = "Unknown title";

    pos = title.indexOf( "content=\"", pos );
    if ( pos < 0 )
        title = "Unknown title";

    title.remove( 0, pos+9 );

    pos = title.indexOf( "\">" );
    if ( pos < 0 )
        title = "Unknown title";

    title.remove( pos, title.size() - pos );

    title = QUrl::fromPercentEncoding(title.toLatin1());

    QString temp_page = page_content;

    pos = temp_page.indexOf("url_encoded_fmt_stream_map");
    if ( pos < 0 )
        return false;

    pos = temp_page.indexOf( "\":", pos);
    if ( pos < 0 )
        return false;

    temp_page.remove( 0, pos );

    pos = temp_page.indexOf( "</script>" );
    if ( pos < 0 )
        return false;

    temp_page.remove( pos, temp_page.size() - pos );

    QString url = temp_page;
    QString sig = temp_page;
    QString itag = temp_page;
    QStringList links;
    QStringList signatures;
    QStringList itags;


    while(1)
    {

        pos = url.indexOf("url=");
        if ( pos < 0 )
            break;

        pos = url.indexOf( "http", pos);
        if ( pos < 0 )
            break;

        url.remove( 0, pos );

        pos = url.indexOf( "\\u0026" );
        int pos2 = url.indexOf( ",sig=" );

        int val[2] = {pos,pos2};

        int nearest = SimplyFunctions::getMinimum(val,2);

        if ( pos < 0 )
            break;

        QString tmp = url;

        tmp.remove( nearest, tmp.size() - nearest );

        links.append(tmp);

        url.remove(0, nearest);


    }


    while(1)
    {
        bool encrypted = false;
        pos = sig.indexOf("sig=");

        if(pos < 0)
        {
           //Encoded signature
            pos = sig.indexOf("\\u0026s=");

             if (pos < 0)
                 break;

             encrypted = true;

        }

      //  qDebug() << "Pos: " << pos;

       // int valu[2] = {pos0,pos};

       // int near1 = SimplyFunctions::getMinimum(valu,2);
        //qDebug() << near1;

       // if (near1 < 0)
       //     break;

        sig.remove( 0, pos );
        sig.remove( 0, sig.indexOf("=")+1);

         //   if (pos < 0)
         //       break;


          //  qDebug() << sig[40];
          /*  if(sig[40] != '.')
            {
               // qDebug() << "Continue";
                continue;
            }
*/

     //   }
     //   else
     //   {
        //    sig.remove( 0, pos+4 );
          //  qDebug() << sig[40];
         /*   if(sig[40] != '.')
            {
          //      qDebug() << "Continue";
                continue;
            }
*/
     //   }




        pos = sig.indexOf( "\\u0026" );// > 0 ? sig.indexOf( "\\u0026" )+6 : -1;
        int pos2 = sig.indexOf( "," );// > 0 ? sig.indexOf( "," )+1 : -1;

        if ( pos < 0 )
            break;

        int val[2] = {pos,pos2};

        int nearest = SimplyFunctions::getMinimum(val,2);

        QString tmp = sig;

        tmp.remove( nearest, tmp.size() - nearest );
       // qDebug() << "Signature: " << tmp;
       // qDebug() << "Signature length: " << tmp.size();

        if(encrypted)
        {
            signatures.append(decryptSignature(tmp));
            qDebug() << "Zaszyfrowane: " << tmp << "\nOdszyfrowane: " << decryptSignature(tmp);
        }
        else
        {
            signatures.append(tmp);
           // qDebug() << tmp;
        }

        //qDebug() << tmp;

        sig.remove(0, nearest);


    }

    bool add = true;
    //QString last = "ta";

    while(1)
    {


        pos = itag.indexOf("itag=");
        if ( pos < 0 )
            break;

        itag.remove( 0, pos+5 );

        pos = itag.indexOf("\"");// > 0 ? itag.indexOf("\"")+1 : -1;
        int pos2 = itag.indexOf(",");// > 0 ? itag.indexOf(",")+1 : -1;
        int pos3 = itag.indexOf("\\u0026");// > 0 ? itag.indexOf("\\u0026")+6 : -1;
        int pos4 = itag.indexOf("&");// > 0 ? itag.indexOf("&")+1 : -1;

        //if (pos < 0)
        //    break;

        int val[4] = {pos,pos2,pos3,pos4};

        int nearest = SimplyFunctions::getMinimum(val,4);
      //  qDebug() << nearest;

        QString tmp = itag;

        tmp.remove( nearest, tmp.size() - nearest );
        if(add)
        {
            itags.append(tmp);
            add = false;
        }
        else
            add = true;

        itag.remove(0, nearest);
       // last = tmp;
       // qDebug() << "Itag: " << tmp;


    }

   // for(int sd=0;sd < itags.size(); sd++)
   //     qDebug() << "Itag: " << itags[sd];

    int v[2] = {links.size(),signatures.size()};
    int min = SimplyFunctions::getMinimum(v,2);
   // QString d;
  //  qDebug() << "Chuj";
    for(int i = 0; i < min; i++)
    {
      //  qDebug() << links[i] << "\n" << signatures.size();
       if(validateItag(itags[i])) //Tylko mp4
       {
            YOUTUBE_FILE file;
            file.fileURL = links[i] + "&signature=" + signatures[i] + "&title=" + title;
            file.signature = signatures[i];
        //    qDebug() << file.fileURL;
            file.itag = itags[i];
            file.quality = itagToQuality(itags[i]);
            files.append(file);
           // qDebug() << file.fileURL << file.signature << file.itag;

        }

           // d += links[i] + "&signature=" + signatures[i] + "&title=" + title + "\n";
    }
   // qDebug() << "Chuj";
/*
        QFile qss("path.txt");
        qss.open(QIODevice::WriteOnly);
        qss.write(d.toAscii());
        qss.close();
*/

    return true;

}

QList<YOUTUBE_FILE> QYouTubeParser::getFileLinks()
{
    return files;
}

QString QYouTubeParser::getTitle()
{
    return title;
}

QString QYouTubeParser::getTitle(QString link)
{


        QString page_content = pageContentV;

        title = page_content;
        // qDebug() << title;
        int pos = title.indexOf("<meta name=\"title\"");
        if ( pos < 0 )
            title = "Unknown title";

        pos = title.indexOf( "content=\"", pos );
        if ( pos < 0 )
            title = "Unknown title";

        title.remove( 0, pos+9 );

        pos = title.indexOf( "\">" );
        if ( pos < 0 )
            title = "Unknown title";

        title.remove( pos, title.size() - pos );

        title = QUrl::fromPercentEncoding(title.toLatin1());

        return title;
}

int QYouTubeParser::getLength()
{
    return length.toInt();
}

int QYouTubeParser::getLength(QString link)
{

        QString page_content = QUrl::fromPercentEncoding(pageContentV.toLatin1());

        length = page_content;
        // qDebug() << title;
        int pos = length.indexOf("length_seconds\":");
        if ( pos < 0 )
            length = "Unknown length";

        length.remove( 0, pos+16 );

        pos = length.indexOf( "," );
        if ( pos < 0 )
            length = "Unknown length";

        length.remove( pos, length.size() - pos );

        //   qDebug() << length;
    return length.toInt();


}


#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <tictactoegame.h>
#include <QScreen>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QScreen * pscreen = app.primaryScreen();
    int  screen_width;
    int  screen_height;
    bool show_expanded(true);
    QString display_mode("FullScreen");
    show_expanded = false; // Uncomment for FULLSCREEN

    if(!show_expanded)
    {
        screen_width = pscreen->size().width();
        screen_height = pscreen->size().height();
    }
    else
    {
        screen_width = pscreen->availableGeometry().width();
        screen_height = pscreen->availableGeometry().height();
        display_mode = "Maximized";
    }
    int temp_width(screen_width);
    if(screen_height > screen_width)
    {
        screen_width = screen_height;
        screen_height = temp_width;
    }

    TicTacToeGame     ttt_game(screen_width, screen_height,&app);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ScreenWidth",screen_width);
    engine.rootContext()->setContextProperty("ScreenHeight",screen_height);
    engine.rootContext()->setContextProperty("TicTacToeGame", &ttt_game);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

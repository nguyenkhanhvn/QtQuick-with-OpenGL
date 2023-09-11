#ifndef FBO_H
#define FBO_H

#include <QQuickFramebufferObject>

class FBO : public QQuickFramebufferObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CustomView3D)
public:
    Renderer *createRenderer() const;
};

#endif // FBO_H

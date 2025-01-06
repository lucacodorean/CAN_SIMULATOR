#ifndef FRAME_H
#define FRAME_H

    #include <windows.h>
    #include <QDebug>

    class Frame
    {
        public:
            Frame();
            Frame(WORD originNode, WORD destinationNode, BOOL ACK_BIT, DWORD data, WORD eof, BYTE priority);

            WORD    getOriginNode();
            WORD    getDestinationNode();
            DWORD   getData();
            BOOL    getAck();
            BYTE    getPriority();
            QString toPipeStructure();

            QString toString();

        private:
            WORD destinationNode, originNode;
            DWORD data;
            BOOL ACK_BIT;
            WORD end_of_frame;
            BYTE priority;
    };

#endif // FRAME_H

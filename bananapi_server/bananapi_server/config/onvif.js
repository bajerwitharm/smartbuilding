module.exports = function () {
    return [
        {
            url: 'rtsp://192.168.1.51:554/streaming/video0',
            name: 'KameraWejscie',
            serialId: '0012123972ab',
            exec: 0,
            timeout: 0
        },
        {
            url: 'rtsp://192.168.1.52:554/streaming/video0',
            name: 'KameraParter',
            serialId: '0012123972cd',
            exec: 0,
            timeout: 0
        },
        {
            url: 'rtsp://192.168.1.53:554/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream',
            name: 'KameraPietro1',
            serialId: '0012123972ca',
            exec: 0,
            timeout: 0
        },
        {
            url: 'rtsp://192.168.1.54:554/user=admin_password=FaWsG5QU_channel=1_stream=0.sdp?real_stream',
            name: 'KameraPietro2',
            serialId: '001212399c78',
            exec: 0,
            timeout: 0
        }
    ];

}
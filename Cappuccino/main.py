import asyncio
import websockets


async def echo(websocket, path):
    async for message in websocket:
        message = 'check'
        await websocket.send(message)


asyncio.get_event_loop().run_until_complete(websockets.serve(echo, '0.0.0.0', 9876))
asyncio.get_event_loop().run_forever()

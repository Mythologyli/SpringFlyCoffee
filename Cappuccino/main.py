import asyncio
import websockets


async def echo(websocket, path):
    async for message in websocket:
        print(message)
        await websocket.send('check')


asyncio.get_event_loop().run_until_complete(websockets.serve(echo, '0.0.0.0', 9876))
asyncio.get_event_loop().run_forever()

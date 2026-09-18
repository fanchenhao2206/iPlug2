from http.server import HTTPServer, SimpleHTTPRequestHandler

SimpleHTTPRequestHandler.extensions_map[".wasm"] = "application/wasm"

port = 8001

server = HTTPServer(("localhost", port), SimpleHTTPRequestHandler)

print(f"Serving at http://localhost:{port}/")
server.serve_forever()

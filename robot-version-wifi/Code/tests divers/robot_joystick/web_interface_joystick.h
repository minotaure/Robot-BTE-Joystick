#define HTML_CODE "<!DOCTYPE html>\n"\
"<html>\n"\
"<head>\n"\
"    <title>\n"\
"        Mousebot\n"\
"    </title>\n"\
"    <meta name=\"viewport\" content=\"user-scalable=no\">\n"\
"</head>\n"\
"<body  style=\"position: fixed; font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif ;\n"\
"color:rgb(128, 128, 128);\n"\
"font-size: xx-large;\">\n"\
"    <h1 style=\"text-align:center\">\n"\
"        MOUSEBOT </h1>\n"\
"    <p style=\"text-align: center;\">\n"\
"        X: <span id=\"x_coordinate\"> </span>\n"\
"        Y: <span id=\"y_coordinate\"> </span>\n"\
"        Speed: <span id=\"speed\"> </span> %\n"\
"        Angle: <span id=\"angle\"> </span>\n"\
"    </p>\n"\
"    <canvas id=\"canvas\" name=\"game\"></canvas>\n"\
"    <script>\n"\
"        var connection = new WebSocket('ws://' + \"192.168.4.1\" + ':81/', ['arduino']);\n"\
"        connection.onopen = function () {\n"\
"            connection.send('Connect ' + new Date());\n"\
"        };\n"\
"        connection.onerror = function (error) {\n"\
"            console.log('WebSocket Error ', error);\n"\
"            alert('WebSocket Error ', error);\n"\
"        };\n"\
"        connection.onmessage = function (e) {\n"\
"            console.log('Server: ', e.data);\n"\
"        };\n"\
"        function send(x,y,speed,angle){\n"\
"            var data = {\"x\":x,\"y\":y,\"speed\":speed,\"angle\":angle};\n"\
"            data = JSON.stringify(data);\n"\
"            console.log(data);\n"\
"            connection.send(data);\n"\
"        }\n"\
"    </script>\n"\
"    <script>\n"\
"        var canvas, ctx;\n"\
"        window.addEventListener('load', () => {\n"\
"            canvas = document.getElementById('canvas');\n"\
"            ctx = canvas.getContext('2d');          \n"\
"            resize(); \n"\
"            document.addEventListener('mousedown', startDrawing);\n"\
"            document.addEventListener('mouseup', stopDrawing);\n"\
"            document.addEventListener('mousemove', Draw);\n"\
"            document.addEventListener('touchstart', startDrawing);\n"\
"            document.addEventListener('touchend', stopDrawing);\n"\
"            document.addEventListener('touchcancel', stopDrawing);\n"\
"            document.addEventListener('touchmove', Draw);\n"\
"            window.addEventListener('resize', resize);\n"\
"            document.getElementById(\"x_coordinate\").innerText = 0;\n"\
"            document.getElementById(\"y_coordinate\").innerText = 0;\n"\
"            document.getElementById(\"speed\").innerText = 0;\n"\
"            document.getElementById(\"angle\").innerText = 0;\n"\
"        });\n"\
"      \n"\
"        var width, height, radius, x_orig, y_orig;\n"\
"        function resize() {\n"\
"            width = window.innerWidth;\n"\
"            radius = 200;\n"\
"            height = radius * 6.5;\n"\
"            ctx.canvas.width = width;\n"\
"            ctx.canvas.height = height;\n"\
"            background();\n"\
"            joystick(width / 2, height / 3);\n"\
"        }\n"\
"        function background() {\n"\
"            x_orig = width / 2;\n"\
"            y_orig = height / 3;\n"\
"            ctx.beginPath();\n"\
"            ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);\n"\
"            ctx.fillStyle = '#ECE5E5';\n"\
"            ctx.fill();\n"\
"        }\n"\
"        function joystick(width, height) {\n"\
"            ctx.beginPath();\n"\
"            ctx.arc(width, height, radius, 0, Math.PI * 2, true);\n"\
"            ctx.fillStyle = '#F08080';\n"\
"            ctx.fill();\n"\
"            ctx.strokeStyle = '#F6ABAB';\n"\
"            ctx.lineWidth = 8;\n"\
"            ctx.stroke();\n"\
"        }\n"\
"        let coord = { x: 0, y: 0 };\n"\
"        let paint = false;\n"\
"        function getPosition(event) {\n"\
"            var mouse_x = event.clientX || event.touches[0].clientX;\n"\
"            var mouse_y = event.clientY || event.touches[0].clientY;\n"\
"            coord.x = mouse_x - canvas.offsetLeft;\n"\
"            coord.y = mouse_y - canvas.offsetTop;\n"\
"        }\n"\
"        function is_it_in_the_circle() {\n"\
"            var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));\n"\
"            if (radius >= current_radius) return true\n"\
"            else return false\n"\
"        }\n"\
"        function startDrawing(event) {\n"\
"            paint = true;\n"\
"            getPosition(event);\n"\
"            if (is_it_in_the_circle()) {\n"\
"                ctx.clearRect(0, 0, canvas.width, canvas.height);\n"\
"                background();\n"\
"                joystick(coord.x, coord.y);\n"\
"                Draw();\n"\
"            }\n"\
"        }\n"\
"        function stopDrawing() {\n"\
"            paint = false;\n"\
"            ctx.clearRect(0, 0, canvas.width, canvas.height);\n"\
"            background();\n"\
"            joystick(width / 2, height / 3);\n"\
"            document.getElementById(\"x_coordinate\").innerText = 0;\n"\
"            document.getElementById(\"y_coordinate\").innerText = 0;\n"\
"            document.getElementById(\"speed\").innerText = 0;\n"\
"            document.getElementById(\"angle\").innerText = 0;\n"\
"        }\n"\
"        function Draw(event) {\n"\
"            if (paint) {\n"\
"                ctx.clearRect(0, 0, canvas.width, canvas.height);\n"\
"                background();\n"\
"                var angle_in_degrees,x, y, speed;\n"\
"                var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));\n"\
"                if (Math.sign(angle) == -1) {\n"\
"                    angle_in_degrees = Math.round(-angle * 180 / Math.PI);\n"\
"                }\n"\
"                else {\n"\
"                    angle_in_degrees =Math.round( 360 - angle * 180 / Math.PI);\n"\
"                }\n"\
"                if (is_it_in_the_circle()) {\n"\
"                    joystick(coord.x, coord.y);\n"\
"                    x = coord.x;\n"\
"                    y = coord.y;\n"\
"                }\n"\
"                else {\n"\
"                    x = radius * Math.cos(angle) + x_orig;\n"\
"                    y = radius * Math.sin(angle) + y_orig;\n"\
"                    joystick(x, y);\n"\
"                }\n"\
"            \n"\
"                getPosition(event);\n"\
"                var speed =  Math.round(100 * Math.sqrt(Math.pow(x - x_orig, 2) + Math.pow(y - y_orig, 2)) / radius);\n"\
"                var x_relative = Math.round(x - x_orig);\n"\
"                var y_relative = Math.round(y - y_orig);\n"\
"                \n"\
"                document.getElementById(\"x_coordinate\").innerText =  x_relative;\n"\
"                document.getElementById(\"y_coordinate\").innerText =y_relative ;\n"\
"                document.getElementById(\"speed\").innerText = speed;\n"\
"                document.getElementById(\"angle\").innerText = angle_in_degrees;\n"\
"                send( x_relative,y_relative,speed,angle_in_degrees);\n"\
"            }\n"\
"        } \n"\
"    </script>\n"\
"</body>\n"\
"</html>"

const express = require("express");
const http = require("http");
const mqtt = require("mqtt");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static("public"));

let dadosAtuais = {
    ocupacao: 0,
    capacidade: 5,
    percentual: 0,
    status: "LIVRE"
};

const mqttClient = mqtt.connect(
    "mqtt://broker.emqx.io:1883"
);

mqttClient.on("connect", () => {

    console.log("MQTT conectado");

    mqttClient.subscribe(
        "senac/flowsense/ocupacao"
    );
});

mqttClient.on(
    "message",
    (topic, message) => {

        try {

            const dados =
                JSON.parse(
                    message.toString()
                );

            const percentual =
                (
                    dados.ocupacao /
                    dados.capacidade
                ) * 100;

            let status = "LIVRE";

            if(percentual >= 100){
                status = "LOTADO";
            }
            else if(percentual >= 80){
                status = "ATENÇÃO";
            }

            dadosAtuais = {
                ocupacao: dados.ocupacao,
                capacidade: dados.capacidade,
                percentual,
                status
            };

            console.log(dadosAtuais);

            io.emit(
                "novoDado",
                dadosAtuais
            );

        } catch(error){

            console.log(
                "Erro:",
                error
            );
        }
    }
);

app.get("/dados", (req,res)=>{
    res.json(dadosAtuais);
});

server.listen(
    3000,
    ()=>{
        console.log(
            "Servidor rodando"
        );
    }
);
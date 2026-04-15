// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

import { createMemo, createResource, createSignal, Component } from "solid-js";
import { createStore } from "solid-js/store";
import { createReconnectingWS, createWSState, WSMessage } from "@solid-primitives/websocket";
import { Filterables, FilterSet, LogEntry } from "./data";
import Messages from "./Messages";
import FilterComponent from "./FilterComponent";
import style from "./App.module.scss";
import "./App.scss";

const App: Component = () => {
    const [logs, setLogs] = createStore<LogEntry[]>([]);
    const [filterables, setFilterables] = createSignal<Filterables>({
        processes: [],
        threads: [],
        names: [],
    });
    const [filterSet, setFilterSet] = createSignal<FilterSet>({
        level: new Set(["trace", "debug", "info", "warn", "error", "critical"]),
    });
    const [clients, setClients] = createSignal(0);

    const [rWsPort] = createResource(async () => {
        const response = await fetch("/api/wsport");
        const data = await response.json();
        return data.port as number;
    });

    const websock = createMemo(() => {
        const port = rWsPort();
        if (port == null) return null;
        const ws = createReconnectingWS(`ws://${window.location.hostname}:${port}`);
        const state = createWSState(ws);
        ws.addEventListener("message", (ev) => {
            const data = JSON.parse(ev.data);
            if (data.type === "client-count") {
                setClients(data.count);
            } //
            else if (data.type === "logs") {
                const newlogs = data.logs.map(
                    (v: any) =>
                        ({
                            applicationName: v.app,
                            level: v.lvl,
                            timestamp: new Date(v.tpt),
                            process: v.pid,
                            thread: v.tid,
                            name: v.nam,
                            message: v.msg,
                            source: v.src,
                            line: v.lno,
                            functionName: v.fnc,
                        }) as LogEntry,
                );
                const MAX_LOGS = 5000;
                setLogs((logs) => {
                    const merged = [...logs, ...newlogs].sort((a, b) => a.timestamp.getTime() - b.timestamp.getTime());
                    return merged.length > MAX_LOGS ? merged.slice(merged.length - MAX_LOGS) : merged;
                });

                // filterables の更新
                setFilterables((prev) => {
                    const processes = new Set(prev.processes);
                    const threads = new Set(prev.threads);
                    const names = new Set(prev.names);
                    for (const log of newlogs) {
                        processes.add(log.process);
                        threads.add(log.thread);
                        names.add(log.name);
                    }
                    if (
                        processes.size === prev.processes.length &&
                        threads.size === prev.threads.length &&
                        names.size === prev.names.length
                    ) {
                        return prev;
                    }
                    return {
                        processes: [...processes].sort((a, b) => a - b),
                        threads: [...threads].sort((a, b) => a - b),
                        names: [...names].sort(),
                    };
                });
            }
        });
        ws.addEventListener("open", () => {
            ws.send(JSON.stringify({ type: "subscribe" }));
        });
        return {
            ws,
            state,
            port,
        };
    });

    return (
        <div class={style.app}>
            <FilterComponent
                filterables={filterables()}
                currentFilterSet={filterSet()}
                updateFilterSets={setFilterSet}
                clientsCount={clients()}
                statusIndex={websock()?.state() || 0}
                port={websock()?.port ?? -1}
            />
            <Messages
                logs={logs}
                filter={filterSet()}
            />
        </div>
    );
};

export default App;

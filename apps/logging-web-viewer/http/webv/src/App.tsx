// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

import { createMemo, createResource, createSignal, Component, Show } from "solid-js";
import { createStore } from "solid-js/store";
import { createReconnectingWS, createWSState } from "@solid-primitives/websocket";
import { Filterables, FilterSet, LogEntry } from "./data";
import Messages from "./Messages";
import FilterComponent from "./FilterComponent";
import style from "./App.module.scss";
import "./App.scss";

const App: Component = () => {
    const [logs, setLogs] = createStore<LogEntry[]>([]);
    const [pausedLogs, setPausedLogs] = createSignal<LogEntry[] | null>(null);
    const [filterables, setFilterables] = createSignal<Filterables>({
        processes: [],
        threads: [],
        names: [],
    });
    const [filterSet, setFilterSet] = createSignal<FilterSet>({
        level: new Set(["trace", "debug", "info", "warning", "error", "critical"]),
    });
    const [autoScroll, setAutoScroll] = createSignal(true);
    let messagesEl: HTMLDivElement | undefined;

    const [clients, setClients] = createSignal(0);

    const [rWsPort] = createResource(async () => {
        return 32124; // TEMP

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
                const MAX_LOGS = 20000;
                setLogs((logs) => {
                    // logs, newlogs はともにソート済み.
                    const merged = [...logs];
                    let i = 0;
                    for (; i < newlogs.length; i++) {
                        const log = newlogs[i];
                        // merged の後ろから log.timestamp より古いものを探す
                        let insertIdx = merged.length;
                        while (insertIdx > 0 && merged[insertIdx - 1].timestamp.getTime() > log.timestamp.getTime()) {
                            insertIdx--;
                        }
                        // もしも挿入場所が末尾だった場合は, for を break する.
                        if (insertIdx === merged.length) {
                            break;
                        }
                        // その場所に log を挿入する.
                        merged.splice(insertIdx, 0, log);
                    }
                    // 残った newlogs について, 単純に merged に連結する.
                    merged.push(...newlogs.slice(i));

                    // 最後切り詰める
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

                if (autoScroll()) {
                    queueMicrotask(() => {
                        if (messagesEl) messagesEl.scrollTop = messagesEl.scrollHeight;
                    });
                }
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

    const stopLogs = (stop: boolean) => {
        if (stop) {
            console.log("PAUSE!");
            setPausedLogs([...logs]);
        } else {
            setPausedLogs(null);
        }
    };

    return (
        <div class={style.app}>
            <FilterComponent
                filterables={filterables()}
                currentFilterSet={filterSet()}
                updateFilterSets={setFilterSet}
                clientsCount={clients()}
                statusIndex={websock()?.state() || 0}
                port={websock()?.port ?? -1}
                autoScroll={autoScroll()}
                setAutoScroll={setAutoScroll}
                stopLogs={stopLogs}
            />
            <Show
                when={pausedLogs() !== null}
                fallback={
                    <Messages
                        logs={logs}
                        filter={filterSet()}
                        ref={(el) => (messagesEl = el)}
                    />
                }
            >
                <Messages
                    logs={pausedLogs()!}
                    filter={filterSet()}
                    ref={(el) => (messagesEl = el)}
                />
            </Show>
        </div>
    );
};

export default App;

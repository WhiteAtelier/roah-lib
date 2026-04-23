// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

import { Component, createMemo, createSignal, For, Show } from "solid-js";
import { FilterSet, LogEntry } from "./data";
import style from "./Messages.module.scss";

interface HighlightItem {
    thread?: number;
    process?: number;
}

const formatTimestamp = (d: Date): string => {
    const MM = String(d.getMonth() + 1).padStart(2, "0");
    const dd = String(d.getDate()).padStart(2, "0");
    const hh = String(d.getHours()).padStart(2, "0");
    const mm = String(d.getMinutes()).padStart(2, "0");
    const ss = String(d.getSeconds()).padStart(2, "0");
    const sss = String(d.getMilliseconds()).padStart(3, "0");
    return `${MM}/${dd} ${hh}:${mm}:${ss}.${sss}`;
};

const filterMessage = (log: LogEntry, filter: FilterSet): boolean => {
    const flg = (v?: boolean) => v !== false;
    return (
        flg(filter.process?.has(log.process)) &&
        flg(filter.thread?.has(log.thread)) &&
        flg(filter.level?.has(log.level)) &&
        flg(filter.name?.has(log.name))
    );
};

const Message: Component<{
    log: LogEntry;
    filter: FilterSet;
    hlt: HighlightItem;
    setHighlighter: (item: HighlightItem) => void;
}> = (props) => {
    const trStyle = createMemo(() => {
        switch (props.log.level) {
            case "trace":
                return style.levelTrace;
            case "debug":
                return style.levelDebug;
            case "info":
                return style.levelInfo;
            case "warning":
                return style.levelWarn;
            case "error":
                return style.levelError;
            case "critical":
                return style.levelCritical;
            default:
                return "";
        }
    });
    const [showDetails, setShowDetails] = createSignal(false);
    const onProcessHover = () => {
        props.setHighlighter({ process: props.log.process });
    };
    const onThreadHover = () => {
        props.setHighlighter({ thread: props.log.thread });
    };
    const onProcessLeave = () => {
        if (props.hlt.process === props.log.process) {
            props.setHighlighter({ process: undefined, thread: props.hlt.thread });
        }
    };
    const onThreadLeave = () => {
        if (props.hlt.thread === props.log.thread) {
            props.setHighlighter({ process: props.hlt.process, thread: undefined });
        }
    };
    return (
        <>
            <tr
                class={`${style.message} ${trStyle()}`}
                onDblClick={() => setShowDetails(!showDetails())}
            >
                <td class={style.level}>{props.log.level}</td>
                <td class={style.timestamp}>{formatTimestamp(props.log.timestamp)}</td>
                <td
                    class={`${style.process} ${props.hlt.process === props.log.process ? style.highlight : ""}`}
                    onMouseEnter={onProcessHover}
                    onMouseLeave={onProcessLeave}
                >
                    {props.log.process}
                </td>
                <td
                    class={`${style.thread} ${props.hlt.thread === props.log.thread ? style.highlight : ""}`}
                    onMouseEnter={onThreadHover}
                    onMouseLeave={onThreadLeave}
                >
                    {props.log.thread}
                </td>
                <td class={style.applicationName}>{props.log.applicationName}</td>
                <td class={style.name}>{props.log.name}</td>
                <td class={style.messageContent}>{props.log.message}</td>
            </tr>
            <Show when={showDetails()}>
                <tr class={style.messageDetails}>
                    <td colSpan={6}></td>
                    <td class={style.detail}>
                        <p>
                            Source: {props.log.source}:{props.log.line}
                        </p>
                        <p>Function: {props.log.functionName}</p>
                    </td>
                </tr>
            </Show>
        </>
    );
};

const Messages: Component<{ logs: LogEntry[]; filter: FilterSet; ref?: (el: HTMLDivElement) => void }> = (props) => {
    const [highlighter, setHighlighter] = createSignal<HighlightItem>({});

    return (
        <div
            class={style.messages}
            ref={props.ref}
        >
            <table class={style.messagesTable}>
                <thead>
                    <tr>
                        <th>Level</th>
                        <th>Timestamp</th>
                        <th>Process</th>
                        <th>Thread</th>
                        <th>Application</th>
                        <th>Name</th>
                        <th>Message</th>
                    </tr>
                </thead>
                <tbody>
                    <For each={props.logs.filter((log) => filterMessage(log, props.filter)).slice(-1000)}>
                        {(log) => (
                            <Message
                                log={log}
                                filter={props.filter}
                                hlt={highlighter()}
                                setHighlighter={setHighlighter}
                            />
                        )}
                    </For>
                </tbody>
            </table>
        </div>
    );
};

export default Messages;

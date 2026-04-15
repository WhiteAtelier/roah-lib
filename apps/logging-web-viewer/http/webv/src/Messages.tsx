import { Component, createMemo, createSignal, For, Match, Show, Switch } from "solid-js";
import { FilterSet, LogEntry } from "./data";
import style from "./Messages.module.scss";

const formatTimestamp = (d: Date): string => {
    const MM = String(d.getMonth() + 1).padStart(2, "0");
    const dd = String(d.getDate()).padStart(2, "0");
    const hh = String(d.getHours()).padStart(2, "0");
    const mm = String(d.getMinutes()).padStart(2, "0");
    const ss = String(d.getSeconds()).padStart(2, "0");
    const sss = String(d.getMilliseconds()).padStart(3, "0");
    return `${MM}/${dd} ${hh}:${mm}:${ss}.${sss}`;
};

const Message: Component<{ log: LogEntry; filter: FilterSet }> = (props) => {
    const flg = (v?: boolean) => v !== false;
    const visible = createMemo(() => {
        return (
            flg(props.filter.process?.has(props.log.process)) &&
            flg(props.filter.thread?.has(props.log.thread)) &&
            flg(props.filter.level?.has(props.log.level)) &&
            flg(props.filter.name?.has(props.log.name))
        );
    });
    const trStyle = createMemo(() => {
        switch (props.log.level) {
            case "trace":
                return style.levelTrace;
            case "debug":
                return style.levelDebug;
            case "info":
                return style.levelInfo;
            case "warn":
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
    return (
        <Show when={visible()}>
            <tr
                class={`${style.message} ${trStyle()}`}
                onClick={() => setShowDetails(!showDetails())}
            >
                <td class={style.level}>{props.log.level}</td>
                <td class={style.timestamp}>{formatTimestamp(props.log.timestamp)}</td>
                <td class={style.process}>{props.log.process}</td>
                <td class={style.thread}>{props.log.thread}</td>
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
        </Show>
    );
};

const Messages: Component<{ logs: LogEntry[]; filter: FilterSet }> = (props) => {
    return (
        <div class={style.messages}>
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
                    <For each={props.logs}>
                        {(log) => (
                            <Message
                                log={log}
                                filter={props.filter}
                            />
                        )}
                    </For>
                </tbody>
            </table>
        </div>
    );
};

export default Messages;

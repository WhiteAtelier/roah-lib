// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

import { Component, createSignal, For, Show } from "solid-js";
import { Filterables, FilterSet, logLevel } from "./data";
import style from "./FilterComponent.module.scss";

const Selector: Component<{ label: string; initial?: boolean; onChanged?: (v: boolean) => void }> = (props) => {
    const [selected, setSelected] = createSignal(props.initial ?? true);
    return (
        <div
            class={`${style.selector} ${selected() ? style.selectorSelected : ""}`}
            onClick={() => {
                const newValue = !selected();
                setSelected(newValue);
                props.onChanged?.(newValue);
            }}
        >
            {props.label}
        </div>
    );
};

const Toggler: Component<{ label: string; value?: boolean; setValue?: (v: boolean) => void }> = (props) => {
    return (
        <div
            class={`${style.selector} ${props.value ? style.selectorSelected : ""}`}
            onClick={() => {
                props.setValue?.(!props.value);
            }}
        >
            {props.label}
        </div>
    );
};

const TabSelector: Component<{ name?: string; currentName?: string; setter?: (name?: string) => void }> = (props) => {
    return (
        <div
            class={`${style.tabSelector} ${props.name === props.currentName ? style.tabSelectorSelected : ""}`}
            onClick={() => props.setter?.(props.name)}
        >
            {props.name || "ALL"}
        </div>
    );
};

const FilterComponent: Component<{
    filterables: Filterables;
    currentFilterSet: FilterSet;
    updateFilterSets: (filterSets: FilterSet) => void;
    clientsCount?: number;
    statusIndex: number;
    port: number;
    autoScroll: boolean;
    setAutoScroll: (v: boolean) => void;
    stopLogs: (stop: boolean) => void;
}> = (props) => {
    const [selectedTab, setSelectedTab] = createSignal<string | undefined>();

    const updateLevelFilter = (level: logLevel, enabled: boolean) => {
        const newLevelSet = new Set(props.currentFilterSet.level);
        if (enabled) {
            newLevelSet.add(level);
        } else {
            newLevelSet.delete(level);
        }
        props.updateFilterSets({ ...props.currentFilterSet, level: newLevelSet });
    };

    const updateNameFilterSelection = (name?: string) => {
        if (name != null) {
            props.updateFilterSets({ ...props.currentFilterSet, name: new Set([name]) });
        } else {
            props.updateFilterSets({ ...props.currentFilterSet, name: undefined });
        }
        setSelectedTab(name);
        props.updateFilterSets(props.currentFilterSet);
    };

    const states = ["Connecting", "Connected", "Disconnecting", "Disconnected"];

    return (
        <div class={style.filterComponent}>
            <div class={style.filters}>
                <div class={style.filterGroup}>
                    <div>COMMANDS: </div>
                    <Toggler
                        label="Auto Scroll"
                        value={props.autoScroll}
                        setValue={props.setAutoScroll}
                    />
                    <Selector
                        label="Stop"
                        initial={false}
                        onChanged={props.stopLogs}
                    />
                </div>
                <div class={style.filterGroup}>
                    <div>LEVELS: </div>
                    <Selector
                        label="trace"
                        onChanged={(v) => updateLevelFilter("trace", v)}
                    />
                    <Selector
                        label="debug"
                        onChanged={(v) => updateLevelFilter("debug", v)}
                    />
                    <Selector
                        label="info"
                        onChanged={(v) => updateLevelFilter("info", v)}
                    />
                    <Selector
                        label="warn"
                        onChanged={(v) => updateLevelFilter("warn", v)}
                    />
                    <Selector
                        label="error"
                        onChanged={(v) => updateLevelFilter("error", v)}
                    />
                    <Selector
                        label="critical"
                        onChanged={(v) => updateLevelFilter("critical", v)}
                    />
                </div>
                <Show when={props.clientsCount != null}>
                    <div class={style.clientsInfo}>
                        <p>State: {states[props.statusIndex]}</p>
                        <p>
                            Server: {window.location.hostname}:{props.port}
                        </p>
                        <p>
                            {props.clientsCount} client{props.clientsCount === 1 ? "" : "s"} online
                        </p>
                    </div>
                </Show>
            </div>
            <div class={style.tabs}>
                <TabSelector
                    currentName={selectedTab()}
                    setter={updateNameFilterSelection}
                />
                <For each={props.filterables.names}>
                    {(name) => (
                        <TabSelector
                            name={name}
                            currentName={selectedTab()}
                            setter={updateNameFilterSelection}
                        />
                    )}
                </For>
            </div>
        </div>
    );
};

export default FilterComponent;

// Copyright (C) 2026 White Atelier
// This software is released under the MIT License.
// See the LICENSE file in the project root for more details.

export type logLevel = "trace" | "debug" | "info" | "warn" | "error" | "critical";

export interface LogEntry {
    applicationName: string;
    timestamp: Date;
    process: number;
    thread: number;
    level: logLevel;
    name: string;
    message: string;
    source: string;
    line: number;
    functionName: string;
}

export interface Filterables {
    processes: number[];
    threads: number[];
    names: string[];
}

export interface FilterSet {
    process?: Set<number>;
    thread?: Set<number>;
    level?: Set<logLevel>;
    name?: Set<string>;
}

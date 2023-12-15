/** @noSelfInFile */

/**
 * A native extension for Defold for generating perlin noise.
 * @noResolution
 */
declare namespace perlin {
	export function noise_1d(
		x: number,
		frequency: number,
		amplitude: number,
		octaveCount: number,
		persistence: number,
	): number;
	export function noise_2d(
		x: number,
		y: number,
		frequency: number,
		amplitude: number,
		octaveCount: number,
		persistence: number,
	): number;
	export function noise_3d(
		x: number,
		y: number,
		z: number,
		frequency: number,
		amplitude: number,
		octaveCount: number,
		persistence: number,
	): number;
	export function noise_4d(
		x: number,
		y: number,
		z: number,
		w: number,
		frequency: number,
		amplitude: number,
		octaveCount: number,
		persistence: number,
	): number;
}
